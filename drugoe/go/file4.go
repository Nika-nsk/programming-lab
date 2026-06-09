package main

import (
	"fmt"
	"sync"
)

type MetroTurnstile struct {
	passengers int
	mu    sync.Mutex
}
func (p *MetroTurnstile) EnterSafe() {
	p.mu.Lock()
	p.passengers++
	p.mu.Unlock()
}

type BrokenTurnstile struct {
	passengers int
}

func (bp *BrokenTurnstile) EnterUnsafe() {
	bp.passengers++
}

func main() {
	const totalpeople = 1000
	var wg sync.WaitGroup
	 goodTurnstile := MetroTurnstile{}
	for i:= 0; i < totalpeople; i++ {
		wg.Add(1)
		go func(){
				defer wg.Done()
				goodTurnstile.EnterSafe()
		}()
	}
wg.Wait()
fmt.Printf("Турникет 1: %d\n",goodTurnstile.passengers)

badTurnstile :=BrokenTurnstile{}
for i := 0; i< totalpeople; i++ {
		wg.Add(1)
		go func(){
			defer wg.Done()
			badTurnstile.EnterUnsafe()
		}()
}
wg.Wait()
fmt.Printf("Турникет 2: %d\n",badTurnstile.passengers)
}