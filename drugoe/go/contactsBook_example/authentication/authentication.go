package app

import (
	"contactsBook/models"
	"contactsBook/utils"
	"context"
	"github.com/dgrijalva/jwt-go"
	"net/http"
	"os"
	"strings"
)

func JwtAuthentication(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		publicRoutes := map[string]bool{
			"/user/new":   true,
			"/user/login": true,
		}

		if publicRoutes[r.URL.Path] {
			next.ServeHTTP(w, r)
			return
		}

		tokenHeader := r.Header.Get("Authorization") //Authorization: Bearer <token>
		if tokenHeader == "" {
			respondForbidden(w, "Missing authentication token")
			return
		}

		parts := strings.Split(tokenHeader, " ")
		if len(parts) != 2 || parts[0] != "Bearer" {
			respondForbidden(w, "Invalid authentication header")
			return
		}

		tokenPart := parts[1]
		claims := &models.Token{}

		token, err := jwt.ParseWithClaims(tokenPart, claims, func(token *jwt.Token) (interface{}, error) {
			return []byte(os.Getenv("token_pass")), nil
		})
		if err != nil {
			respondForbidden(w, "Malformed authentication token")
			return
		}

		if !token.Valid {
			respondForbidden(w, "Token is not valid")
			return
		}

		ctx := context.WithValue(r.Context(), "user", claims.UserId)
		next.ServeHTTP(w, r.WithContext(ctx))
	})
}

func respondForbidden(w http.ResponseWriter, message string) {
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusForbidden)
	utils.Respond(w, utils.Message(false, message))
}

//GET /me/contacts  Authorization: Bearer abc.def.ghi

/*
step 1
Запрос попадает в router.

step 2
Router вызывает middleware JwtAuthentication.

step 3
Middleware смотрит путь: /me/contacts
Он не публичный.

step 4
Middleware берёт заголовок Authorization.

step 5
Проверяет, что формат Bearer <token>.

step 6
Достаёт токен и парсит его через jwt.ParseWithClaims.

step 7
Если токен валиден, получает из него UserId.

step 8
Кладёт UserId в context.

step 9
Передаёт управление контроллеру.

step 10
Контроллер делает: userID := r.Context().Value("user").(uint)
*/
