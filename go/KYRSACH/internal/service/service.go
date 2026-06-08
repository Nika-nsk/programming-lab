package service //проверка чтобы не было пустых окошек или отрицательных повторений

import (
	"errors"
	"fitness-diary/internal/model"
	"fitness-diary/internal/repository"
)

type Service struct {
	repo *repository.Repository
}


func NewService(repo *repository.Repository) *Service {
	return &Service{repo: repo}
}

func (s *Service) CreateDay(name string) (int, error) {
	if name == "" {
		return 0, errors.New("название дня не может быть пустым")
	}
	return s.repo.CreateDay(name)
}

func (s *Service) DeleteDay(id int) error {
	if id <= 0 {
		return errors.New("неверный ID дня")
	}
	return s.repo.DeleteDay(id)
}

func (s *Service) AddExercise(dayID int, name string, sets, reps int) (int, error) {
	if dayID <= 0 {
		return 0, errors.New("неверный ID дня")
	}
	if name == "" {
		return 0, errors.New("название упражнения не может быть пустым")
	}
	if sets < 0 || reps < 0 {
		return 0, errors.New("подходы и повторения не могут быть меньше нуля")
	}
	return s.repo.AddExercise(dayID, name, sets, reps)
}

func (s *Service) DeleteExercise(id int) error {
	if id <= 0 {
		return errors.New("неверный ID упражнения")
	}
	return s.repo.DeleteExercise(id)
}

func (s *Service) ToggleExercise(id int) error {
	if id <= 0 {
		return errors.New("неверный ID упражнения")
	}
	return s.repo.ToggleExercise(id)
}

func (s *Service) GetAllDays() []model.TrainingDay {
	return s.repo.GetAllDays()
}
