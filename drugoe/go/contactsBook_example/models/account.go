package models

import (
	"contactsBook/utils"
	"github.com/dgrijalva/jwt-go"
	"github.com/jinzhu/gorm"
	"golang.org/x/crypto/bcrypt"
	"os"
	"strings"
)

type Token struct {
	UserId uint
	jwt.StandardClaims
}

type Account struct {
	gorm.Model
	Email    string `json:"email"`
	Password string `json:"password"`
	Token string `json:"token" sql:"-"`
}

func (account *Account) Validate() (map[string]interface{}, bool) {
	if !strings.Contains(account.Email, "@") {
		return utils.Message(false, "Email address is not valid!"), false
	}

	if len(account.Password) < 4 {
		return utils.Message(false, "Password must be longer then 4 symbols"), false
	}

	acc := &Account{}

	err := GetDB().Table("accounts").Where("email = ?", account.Email).First(acc).Error
	if err != nil && err != gorm.ErrRecordNotFound {
		return utils.Message(false, "Connection error!"), false
	}
	if acc.Email != "" {
		return utils.Message(false, "The email is already occupied by another user!"), false
	}

	return utils.Message(true, "Check is passed!"), true
}

func (account *Account) CreateAccount() map[string]interface{} {
	if resp, ok := account.Validate(); !ok {
		return resp
	}

	pwd, _ := bcrypt.GenerateFromPassword([]byte(account.Password), bcrypt.DefaultCost)
	account.Password = string(pwd)

	GetDB().Create(account)

	if account.ID <= 0 {
		return utils.Message(false, "Failed to create account, connection error.")
	}

	tk := &Token{UserId: account.ID}
	token := jwt.NewWithClaims(jwt.GetSigningMethod("HS256"), tk)
	tokenStr, _ := token.SignedString([]byte(os.Getenv("token_pass")))
	account.Token = tokenStr

	account.Password = ""

	response := utils.Message(true, "Account has been created!")
	response["account"] = account
	return response
}

func LoginAccount(email, password string) map[string]interface{} {
	account := &Account{}
	err := GetDB().Table("accounts").Where("email = ?", email).First(account).Error
	if err != nil {
		if err == gorm.ErrRecordNotFound {
			return utils.Message(false, "Email address not found")
		}
		return utils.Message(false, "Connection error. Please retry")
	}

	err = bcrypt.CompareHashAndPassword([]byte(account.Password), []byte(password))
	if err != nil && err == bcrypt.ErrMismatchedHashAndPassword {
		return utils.Message(false, "Password does not match!")
	}
	account.Password = ""
	tk := &Token{UserId: account.ID}
	token := jwt.NewWithClaims(jwt.GetSigningMethod("HS256"), tk)
	tokenStr, _ := token.SignedString([]byte(os.Getenv("token_pass")))
	account.Token = tokenStr

	resp := utils.Message(true, "Logged In")
	resp["account"] = account
	return resp
}

func GetUser(utils uint) *Account {
	acc := &Account{}
	GetDB().Table("accounts").Where("id = ?", utils).First(acc)
	if acc.Email == "" { //User not found!
		return nil
	}

	acc.Password = ""
	return acc
}
