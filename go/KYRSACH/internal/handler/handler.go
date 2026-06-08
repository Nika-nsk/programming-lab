package handler //принимает запросы браузера и отправляет ответы, переводит JSON в Go

import (
	"encoding/json"
	"net/http"
	"strconv"
	"strings"

	"fitness-diary/internal/service"
)

type Handler struct {
	services *service.Service
}

func NewHandler(services *service.Service) *Handler {
	return &Handler{services: services}
}


func (h *Handler) RegisterRoutes(mux *http.ServeMux) {
	mux.HandleFunc("/api/v1/days", h.handleDays)
	mux.HandleFunc("/api/v1/days/", h.handleDaysWithID)
	mux.HandleFunc("/api/v1/exercises/", h.handleExercises)
}


func (h *Handler) handleDays(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")

	switch r.Method {
	case http.MethodGet:
		days := h.services.GetAllDays()
		json.NewEncoder(w).Encode(days)

	case http.MethodPost:
		var input struct {
			Name string `json:"name"`
		}
		if err := json.NewDecoder(r.Body).Decode(&input); err != nil {
			http.Error(w, "Неверный формат JSON", http.StatusBadRequest)
			return
		}

		id, err := h.services.CreateDay(input.Name)
		if err != nil {
			http.Error(w, err.Error(), http.StatusBadRequest)
			return
		}

		w.WriteHeader(http.StatusCreated)
		json.NewEncoder(w).Encode(map[string]int{"id": id})

	default:
		http.Error(w, "Метод не поддерживается", http.StatusMethodNotAllowed)
	}
}


func (h *Handler) handleDaysWithID(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	parts := strings.Split(strings.Trim(r.URL.Path, "/"), "/")

	if len(parts) < 4 {
		http.Error(w, "Неверный URL", http.StatusBadRequest)
		return
	}

	dayID, err := strconv.Atoi(parts[3])
	if err != nil {
		http.Error(w, "Неверный ID дня", http.StatusBadRequest)
		return
	}

	
	if len(parts) == 5 && parts[4] == "exercises" && r.Method == http.MethodPost {
		var input struct {
			Name string `json:"name"`
			Sets int    `json:"sets"`
			Reps int    `json:"reps"`
		}
		if err := json.NewDecoder(r.Body).Decode(&input); err != nil {
			http.Error(w, "Неверный формат JSON", http.StatusBadRequest)
			return
		}

		id, err := h.services.AddExercise(dayID, input.Name, input.Sets, input.Reps)
		if err != nil {
			http.Error(w, err.Error(), http.StatusBadRequest)
			return
		}

		w.WriteHeader(http.StatusCreated)
		json.NewEncoder(w).Encode(map[string]int{"id": id})
		return
	}

	
	if len(parts) == 4 && r.Method == http.MethodDelete {
		if err := h.services.DeleteDay(dayID); err != nil {
			http.Error(w, err.Error(), http.StatusNotFound)
			return
		}
		w.WriteHeader(http.StatusOK)
		json.NewEncoder(w).Encode(map[string]string{"status": "deleted"})
		return
	}

	http.Error(w, "Метод не поддерживается или неверный URL", http.StatusMethodNotAllowed)
}


func (h *Handler) handleExercises(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	parts := strings.Split(strings.Trim(r.URL.Path, "/"), "/")

	if len(parts) < 4 {
		http.Error(w, "Неверный URL", http.StatusBadRequest)
		return
	}

	exID, err := strconv.Atoi(parts[3])
	if err != nil {
		http.Error(w, "Неверный ID упражнения", http.StatusBadRequest)
		return
	}

	
	if r.Method == http.MethodDelete {
		if err := h.services.DeleteExercise(exID); err != nil {
			http.Error(w, err.Error(), http.StatusNotFound)
			return
		}
		w.WriteHeader(http.StatusOK)
		json.NewEncoder(w).Encode(map[string]string{"status": "deleted"})
		return
	}

	
	if r.Method == http.MethodPatch && len(parts) == 5 && parts[4] == "toggle" {
		if err := h.services.ToggleExercise(exID); err != nil {
			http.Error(w, err.Error(), http.StatusNotFound)
			return
		}
		w.WriteHeader(http.StatusOK)
		json.NewEncoder(w).Encode(map[string]string{"status": "toggled"})
		return
	}

	http.Error(w, "Метод не поддерживается", http.StatusMethodNotAllowed)
}
