package controllers

import (
	"contactsBook/models"
	"contactsBook/utils"
	"encoding/json"
	"net/http"
)

func CreateAccount(w http.ResponseWriter, r *http.Request) {
	account := &models.Account{}

	if err := json.NewDecoder(r.Body).Decode(account); err != nil {
		utils.Respond(w, utils.Message(false, "Invalid request"))
		return
	}

	response := account.CreateAccount()
	utils.Respond(w, response)
}

func LoginAccount(w http.ResponseWriter, r *http.Request) {
	account := &models.Account{}

	if err := json.NewDecoder(r.Body).Decode(account); err != nil {
		utils.Respond(w, utils.Message(false, "Invalid request"))
		return
	}

	response := models.LoginAccount(account.Email, account.Password)
	utils.Respond(w, response)
}
