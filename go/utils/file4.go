package utils

import (
	"errors"
	"sort"
)

// FilterAndSort принимает слайс чисел,
// оставляет только числа >= min,
// и возвращает отсортированный результат.

// Если входной слайс nil — возвращает ошибку.
// Если после фильтрации нет элементов — тоже ошибка.

func FilterAndSort(nums []int, min int) ([]int, error) {
	if nums == nil {
		return nil, errors.New("input slice is nil")
	}

	if min < 0 {
		return nil, errors.New("min cannot be negative")
	}

	var filtered []int
	for _, n := range nums {
		if n >= min {
			filtered = append(filtered, n)
		}
	}

	if len(filtered) == 0 {
		return nil, errors.New("no elements after filtering")
	}

	sort.Ints(filtered)

	return filtered, nil
}