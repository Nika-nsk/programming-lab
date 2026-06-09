package main
import "fmt"
func main() {
	var nums []int
	fmt.Print("Заполните массив, nums = ")
for { var n int
	_, err := fmt.Scan(&n)
	if err != nil{
		break
	}
	nums = append(nums, n)
}
var k int
fmt.Print("Задайте элемент, k = ")
_, err := fmt.Scan(&k)
if err != nil {
        fmt.Println("Ошибка: вы ввели не число для k")
    }
	for i := 0; i < len(nums); i++ {
		for j := i + 1; j < len(nums); j++ {
			if nums[i] < nums[j] {
				nums[i], nums[j] = nums[j], nums[i]
			}
		}
	}
fmt.Println("Массив:", nums)
fmt.Printf("%d-й наибольший элемент: %d\n", k, nums[k-1])
}