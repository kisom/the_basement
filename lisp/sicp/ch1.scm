; structure and interpretation of computer programs
; chapter 1 exercises

; exercise 1.1
; Below is a sequence of expressions. What is the result printed by
; the interpreter in response to each expression? Assume that the sequence is to
; be evaluated in the order in which it is presented.
; sexpr                                                     result
; 10                                                        10
; (+ 5 3 4)                                                 12
; (- 9 1)                                                   8
; (/ 6 2)                                                   3
; (+ (* 2 4) (- 4 6))                                       6
; (define a 3)         
; (define b (+ a 1))                    
; (+ a b (* a b))                                           19
; (= a b)                                                   #f
; (if (and (> b a) (< b (* a b)))                           4
;  b
;  a)
; (cond ((= a 4) 6)                                         16
;       ((= b 4) (+ 6 7 a))
;       (else 25))
; (+ 2 (if (> b a) b a))                                    6
; (* (cond ((> a b) a)                                      16
;          ((< a b) b)
;          (else -1))
;    (+ a 1))


; exercise 1.2
; Translate the following expression into prefix form.
;
; 5 + 4 + (2 - (3 - (6 + 4/5)))
; -----------------------------
;       3(6 - 2)(2 - 7)
;
; (/ 
;   (+ 5 4 (- 2 (+ 6 (/ 4 5))))
;   (* 3 (- 6 2) (- 2 7)))


; exercise 1.3: 
; Define a procedure that takes three numbers as arguments and returns the sum 
; of the squares of the two larger numbers.
; 
; could definitely be cleaned up
; adds max and second-largest numbers
(define (f1.3 x y z) 
  (+
   (square (if (> x y z) 
       x 
       (if (> y z) y z)))
   (square (if (> x y z) 
       (if (> y z) y z)
       (if (> y x z) 
           (if (> x z) x z)
           (if (> x y) x y))))))
           

; exercise 1.4
; Observe that our model of evaluation allows for combinations whose operators 
; are compound expressions. Use this observation to describe the behavior of 
; the following procedure:
;
;(define (a-plus-abs-b a b)
;  ((if (> b 0) + -) a b))
;
; the conditional selects the operator to apply to the arguments


; exercise 1.5
; Ben Bitdiddle has invented a test to determine whether the interpreter he is 
; faced with is using applicative-order evaluation or normal-order evaluation. 
; He defines the following two procedures:
;
;(define (p) (p))
;
;(define (test x y)
; (if (= x 0)
;  0
;  y))
;
; Then he evaluates the expression
; (test 0 (p))
;
; What behavior will Ben observe with an interpreter that uses applicative-order 
;evaluation? What behavior will he observe with an interpreter that uses normal-
;order evaluation? Explain your answer. (Assume that the evaluation rule for
;the special form if is the same whether the interpreter is using normal or
;applicative order: The predicate expression is evaluated first, and the result
;determines whether to evaluate the consequent or the alternative expression.)
;
; applicative order:
; (if (= 0 p) 0 (p))
; hangs at this point due to recursive definition of (p)
;
; normal order:
; (if (= 0 p) 0 (p))
; (if #t 0 (p))
; 0


; exercise 1.6
; Alyssa P. Hacker doesn’t see why if needs to be provided as a special form. 
; “Why can’t I just define it as an ordinary procedure in terms of cond?” she 
; asks. Alyssa’s friend Eva Lu Ator claims this can indeed be done, and she 
; defines a new version of if:
;
; (define (new-if predicate then-clause else-clause)
;   (cond (predicate then-clause)
;         (else else-clause)))
;
; Eva demonstrates the program for Alyssa:
; (new-if (= 2 3) 0 5)
; 5
; (new-if (= 1 1) 0 5)
; 0
;
; Delighted, Alyssa uses new-if to rewrite the square-root program:
;
; (define (sqrt-iter guess x)
;         (new-if (good-enough? guess x)
;                  guess
;                  (sqrt-iter (improve guess x)
;                    x)))
; 
; What happens when Alyssa attempts to use this to compute square roots?
; Explain.
;
; the interpreter hangs...

; exercise 1.7
; The good-enough? test used in computing square roots will not
; be very effective for finding the square roots of very small numbers. Also, in
; real computers, arithmetic operations are almost always performed with limited
; precision. This makes our test inadequate for very large numbers. Explain
; these statements, with examples showing how the test fails for small and large
; numbers. An alternative strategy for implementing good-enough? is to watch
; how guess changes from one iteration to the next and to stop when the change
; is a very small fraction of the guess. Design a square-root procedure that uses
; this kind of end test. Does this work better for small and large numbers?

(define tolerance 0.00015)

(define (myabs x)
  ((if (< x 0) - +) x))

(define (good-enough? guess prev-guess x)
  (cond
    ((= guess prev-guess) #t)
    ((< (/ (abs (- prev-guess guess)) guess) tolerance) #t)
    (else #f)))


; exercise 1.8
; Newton’s method for cube roots is based on the fact that if y is
; an approximation to the cube root of x, then a better approximation is given
; by the value
;
; x/y^2 + 2y
; ----------
;     3
;
; Use this formula to implement a cube-root procedure analogous to the square-
; root procedure. (In section Section 1.3.4 [1-3-4], page 69 we will see how to
; implement Newton’s method in general as an abstraction of these square-root
; and cube-root procedures.)
;

(define (cubert-iter guess prev-guess x)
  (if (good-enough? guess prev-guess x)
      guess
      (cubert-iter (improve guess x) guess x)))

(define (improve guess x)
  (/
   (+
    (/ x (square guess))
    (* 2 guess))
   3))

(define (cubert x)
  (cubert-iter 1.0 0.0 x))
