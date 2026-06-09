package main

import (
	auth "contactsBook/authentication"
	"contactsBook/controllers"
	"fmt"
	"github.com/gorilla/mux"
	"net/http"
	"os"
)

func main() {
	router := mux.NewRouter()

	router.HandleFunc("/user/new", controllers.CreateAccount).Methods("POST")
	router.HandleFunc("/user/login", controllers.LoginAccount).Methods("POST")
	router.HandleFunc("/contacts/new", controllers.CreateContact).Methods("POST")
	router.HandleFunc("/me/contacts", controllers.GetContacts).Methods("GET")
	router.HandleFunc("/contacts/{id}", controllers.UpdateContact).Methods("PUT")//Update contact (метод PUT)
	router.HandleFunc("/contacts/{id}", controllers.DeleteContact).Methods("DELETE")//Delete contact (метод DELETE)

	router.Use(auth.JwtAuthentication)

	port := os.Getenv("PORT")
	if port == "" {
		port = "8000"
	}

	fmt.Println("Server started on port:", port)

	err := http.ListenAndServe(":"+port, router)
	if err != nil {
		fmt.Print(err)
	}
}
