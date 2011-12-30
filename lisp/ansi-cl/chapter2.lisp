;;; chapter2.lisp
;;  exercises from ANSI Common Lisp by Paul Graham
;;  2011-12-28
;;  Kyle Isom <kyle@nodality.io>

(defun ask-number ()
  (format t "Please enter a number: ")
  (let ((val (read)))
    (if (numberp val)
        val
        (ask-number))))


;; exercise 2: give three distinct cons expressions that return (a b c)
(cons 'a (cons 'b (cons 'c ())))
(cons 'a (cons 'b (cons 'c nil)))
(list 'a 'b 'c)

;; exercise 3: using car and cdr, define a funcion to return the fourth element
; of a list
(defun my-fourth (lst)
  (car (cdr (cdr (cdr lst)))))

;; exercise 4: define a function that takes two arguments and returns the greater
; of the two
(defun greater (x y)
  (if (> x y) x y))

;; exercise 5: what do these functions do?
; (a)
(defun enigma (x)
  (and (not (null x))
       (or (null (car x))
	   (enigma (cdr x)))))
; scan for empty lists in a list

; (b)
(defun mystery (x y)
  (if (null y)
      nil
      (if (eql (car y) x)
	  0
	  (let ((z (mystery x (cdr y))))
	    (and z (+ z 1))))))
; returns the index of item x in list y if x is in the list, or nil otherwise

;; exercise 6: what could occur in place of x in each of the following 
;  exchanges?

; (a)
(defun test-6a (x)
  (car (funcall x (cdr '(a (b c) d)))))
; if x->car, B
; if x->cdr, D
; if x->list, ((B C) D)
; if x->null, NIL (NIL represents an empty list; anything returning t fails
;    because t is an atom, not a list)

; (b)
;(defun test-6b (x)
;  (funcall x 13 (/ 1 0)))
; if x->and, 13
; NOTE: because and is a macro, it can't be tested with the above function

; (c)
(defun test-6c (x)
  (funcall x #'list 1 nil))
; if x->funcall, (1 NIL)
; if x->apply, (1)

;; exercise 7: using only operators introduced in this chapter, define a
;  function that takes a list as an argument and returns true if one of its
;  elements is a list

(defun has-sublist (lst)
  (if (listp lst)
      (if (listp (car lst))               ; rely on default nil for falsity
	  (car lst)
	  (has-sublist (cdr lst)))))

;; exercise 8: give iterative and recursive and recursive definitions of a
;  function that

; (a) takes a positive integer and prints that many dots
(defun it-dot-print (n)
  (do ((i 1 (+ i 1)))
      ((> i n) (format t "~%"))
    (format t ".")))

(defun rec-dot-print (n)
  (if (eql n 0)
      (format t "~%")
      (progn
	(format t ".")
	(rec-dot-print (- n 1)))))

; (b) takes a list and returns the number of times the symbol a occurs in it
(defun it-find-a (lst)
  (let ((n 0))
    (dolist (obj lst)
      (if (eql obj 'a)
	(setf n (+ n 1))))
    n))

(defun rec-find-a (lst)
  (if (null lst)
      0
      (+ (if (eq (car lst) 'a) 1 0)
	 (rec-find-a (cdr lst)))))

;; exercise 9: a friend is trying to write a function that returns the sum
;  of all the non-nil elements in a list. He has written two versions of 
;  this function, and neither of them work. Explain what's wrong with each,
;  and give a correction version.

; (a)
; friend's version
(defun summit (lst)
  (remove nil lst)
  (apply #'+ lst))
; remove doesn't update list

(defun summit (lst)
  (apply #'+ (remove nil lst)))

; (b)

(defun summit (lst)
  (let ((x (car lst)))
    (if (null x)
	(summit (cdr lst))
        (+ x (summit (cdr lst))))))
; no stop condition for recursion

(defun summit (lst)
  (if (null lst)
      0
      (let ((x (car lst)))
	(+ 
	 (if (null x) 0 x)
	 (summit (cdr lst))))))
