package service //Unit-тесты

import (
	"fitness-diary/internal/repository"
	"testing"
)

func TestCreateDay_ValidationError(t *testing.T) { //чтоб не было пустого названия дня
	
	repo := repository.NewRepository()
	srv := NewService(repo)

	
	_, err := srv.CreateDay("")
	if err == nil {
		t.Error("Ожидалась ошибка при создании дня с пустым именем, но ошибка не получена")
	}
}

func TestAddExercise_NegativeSets(t *testing.T) { //чтобы не было отрицательных упражнений
	repo := repository.NewRepository()
	srv := NewService(repo)


	_, err := srv.AddExercise(1, "Приседания", -5, 10)
	if err == nil {
		t.Error("Ожидалась ошибка при отрицательном количестве подходов, но ошибка не получена")
	}
}
