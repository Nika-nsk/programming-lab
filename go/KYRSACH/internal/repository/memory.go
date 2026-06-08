package repository

import (
	"errors"
	"sync"
	"time"

	"fitness-diary/internal/model"
)

type Repository struct {
	mu        sync.RWMutex
	days      map[int]*model.TrainingDay
	exercises map[int]*model.Exercise
	nextDayID int
	nextExID  int
}

// создает чистое хранилище в памяти
func NewRepository() *Repository {
	return &Repository{
		days:      make(map[int]*model.TrainingDay),
		exercises: make(map[int]*model.Exercise),
		nextDayID: 1,
		nextExID:  1,
	}
}

// создает новый день тренировки
func (r *Repository) CreateDay(name string) (int, error) {
	r.mu.Lock()
	defer r.mu.Unlock()

	id := r.nextDayID
	r.days[id] = &model.TrainingDay{
		ID:        id,
		Name:      name,
		CreatedAt: time.Now(),
		Exercises: []model.Exercise{},
	}
	r.nextDayID++
	return id, nil
}

//  удаляет день и все его упражнения (каскадно)
func (r *Repository) DeleteDay(id int) error {
	r.mu.Lock()
	defer r.mu.Unlock()

	if _, exists := r.days[id]; !exists {
		return errors.New("день тренировки не найден")
	}

	// Удаляем связанные упражнения
	for exID, ex := range r.exercises {
		if ex.TrainingDayID == id {
			delete(r.exercises, exID)
		}
	}

	delete(r.days, id)
	return nil
}

//  добавляет упражнение к конкретному дню
func (r *Repository) AddExercise(dayID int, name string, sets, reps int) (int, error) {
	r.mu.Lock()
	defer r.mu.Unlock()

	if _, exists := r.days[dayID]; !exists {
		return 0, errors.New("день тренировки не найден")
	}

	id := r.nextExID
	r.exercises[id] = &model.Exercise{
		ID:            id,
		TrainingDayID: dayID,
		Name:          name,
		Sets:          sets,
		Reps:          reps,
		IsCompleted:   false,
	}
	r.nextExID++
	return id, nil
}

//  удаляет конкретное упражнение
func (r *Repository) DeleteExercise(id int) error {
	r.mu.Lock()
	defer r.mu.Unlock()

	if _, exists := r.exercises[id]; !exists {
		return errors.New("упражнение не найдено")
	}

	delete(r.exercises, id)
	return nil
}

// ToggleExercise переключает статус выполнения (выполнено / не выполнено)
func (r *Repository) ToggleExercise(id int) error {
	r.mu.Lock()
	defer r.mu.Unlock()

	ex, exists := r.exercises[id]
	if !exists {
		return errors.New("упражнение не найдено")
	}

	ex.IsCompleted = !ex.IsCompleted
	return nil
}

// GetAllDays возвращает все дни вместе с их упражнениями
func (r *Repository) GetAllDays() []model.TrainingDay {
	r.mu.RLock()
	defer r.mu.RUnlock()

	result := make([]model.TrainingDay, 0, len(r.days))

	for _, day := range r.days {
		// Собираем упражнения для этого дня
		dayCopy := *day
		dayCopy.Exercises = []model.Exercise{}
		
		for _, ex := range r.exercises {
			if ex.TrainingDayID == day.ID {
				dayCopy.Exercises = append(dayCopy.Exercises, *ex)
			}
		}
		result = append(result, dayCopy)
	}

	return result
}
