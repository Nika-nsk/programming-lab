package model

import "time"


type TrainingDay struct {
	ID        int        `json:"id"`
	Name      string     `json:"name"`
	CreatedAt time.Time  `json:"created_at"`
	Exercises []Exercise `json:"exercises,omitempty"`
}


type Exercise struct {
	ID            int    `json:"id"`
	TrainingDayID int    `json:"training_day_id"`
	Name          string `json:"name"`
	Sets          int    `json:"sets"`
	Reps          int    `json:"reps"`
	IsCompleted   bool   `json:"is_completed"`
}
