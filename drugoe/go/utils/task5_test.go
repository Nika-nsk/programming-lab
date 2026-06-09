package utils_test

import (
	"testing"
	"task5_example/utils"
)

// вспомогательная функция сравнения слайсов
func equalSlices(a, b []int) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if a[i] != b[i] {
			return false
		}
	}
	return true
}

func TestFilterAndSort(t *testing.T) {
	t.Run("ValidData", func(t *testing.T) {
		input := []int{10, 2, 5, 8, 1}
		min := 5
		expected := []int{5, 8, 10}
		got, err := utils.FilterAndSort(input, min)
		if err != nil {
			t.Fatalf("Unexpected error: %v", err)
		}
		if !equalSlices(got, expected) {
			t.Errorf("Expected %v, got %v", expected, got)
		}
	})
	t.Run("NilInput", func(t *testing.T) {
		_, err := utils.FilterAndSort(nil, 0)
		if err == nil {
			t.Error("Expected error for nil input, got nil")
		}
	})
	t.Run("NegativeMin", func(t *testing.T){
		_, err := utils.FilterAndSort([]int{1}, -5)
		if err == nil {
			t.Error("Expected error for negative min, got nil")
	}
})
	t.Run("EmptyResult", func(t *testing.T) {
		_, err := utils.FilterAndSort([]int{1, 2, 3}, 10)
		if err == nil {
			t.Error("Expected error for empty result, got nil")
		}
	})
}
func FuzzFilterAndSort(f *testing.F) {
	f.Add([]byte{1, 2, 3}, 5) 
	f.Fuzz(func(t *testing.T, data []byte, min int) {
		
		var nums []int
		for _, b := range data {
			nums = append(nums, int(b))
		}
		
		
		utils.FilterAndSort(nums, min)
	})
}