; ch1 example code

; square a number
(define (square x) 
  (* x x))

; newton's method for finding square roots
(define (sqrt-iter guess prev-guess x)
  (if (good-enough? guess prev-guess x)
      guess
      (sqrt-iter (improve guess x) guess x)))

(define (improve guess x)
  (average guess (/ x guess)))

(define (average x y)
  (/ (+ x y) 2)) 

(define (good-enough? guess x)
  (< (abs (- (square guess) x)) 0.001))

(define (sicpsqrt x) 
  (sqrt-iter 1.0 0.0 x))


; test exercises here
