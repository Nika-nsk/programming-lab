package main

import (
	"log"
	"net/http"

	"fitness-diary/internal/config"
	"fitness-diary/internal/handler"
	"fitness-diary/internal/repository"
	"fitness-diary/internal/service"
)

func main() {
	cfg := config.LoadConfig()

	repo := repository.NewRepository()
	services := service.NewService(repo)
	handlers := handler.NewHandler(services)

	mux := http.NewServeMux()
	
	
	mux.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		http.ServeFile(w, r, "cmd/app/index.html")
	})

	handlers.RegisterRoutes(mux)

	log.Printf("Сервер фитнес-дневника успешно запущен на порту %s", cfg.ServerPort)
	
	if err := http.ListenAndServe(cfg.ServerPort, mux); err != nil {
		log.Fatalf("Ошибка при запуске сервера: %s", err.Error())
	}
}
