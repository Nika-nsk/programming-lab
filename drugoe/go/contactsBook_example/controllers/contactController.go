package controllers

import (
	"contactsBook/models"
	"contactsBook/utils"
	"encoding/json"
	"net/http"
	"strconv" 
	"github.com/gorilla/mux"
)

func CreateContact(w http.ResponseWriter, r *http.Request) {
	defer r.Body.Close()

	userID, ok := r.Context().Value("user").(uint)
	if !ok {
		utils.Respond(w, utils.Message(false, "Unauthorized"))
		return
	}

	contact := &models.Contact{}

	if err := json.NewDecoder(r.Body).Decode(contact); err != nil {
		utils.Respond(w, utils.Message(false, "Invalid request body"))
		return
	}

	contact.UserId = userID

	utils.Respond(w, contact.CreateContact())
}

func GetContacts(w http.ResponseWriter, r *http.Request) {
	userID, ok := r.Context().Value("user").(uint)
	if !ok {
		utils.Respond(w, utils.Message(false, "Unauthorized"))
		return
	}

	data := models.GetContacts(userID)

	response := utils.Message(true, "success")
	response["data"] = data

	utils.Respond(w, response)
}

func UpdateContact(w http.ResponseWriter, r *http.Request) {
	params := mux.Vars(r)
	id, _ := strconv.Atoi(params["id"])
	userID, ok := r.Context().Value("user").(uint)
	if !ok {
		utils.Respond(w, utils.Message(false, "Unauthorized"))
		return
	}
	contact := &models.Contact{}
	json.NewDecoder(r.Body).Decode(contact)
	contact.UserId = userID
	utils.Respond(w, contact.Update(uint(id)))
}

func DeleteContact(w http.ResponseWriter, r *http.Request) {
	params := mux.Vars(r)
	id, _ := strconv.Atoi(params["id"])
	userID, ok := r.Context().Value("user").(uint)
	if !ok {
		utils.Respond(w, utils.Message(false, "Unauthorized"))
		return
	}
	utils.Respond(w, models.DeleteContact(uint(id), userID))
}
