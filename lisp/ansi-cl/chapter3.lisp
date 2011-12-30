;;; chapter3.lisp
;;  exercises from chapter 3 of "ANSI Common Lisp" by Paul Graham
;;  2011-12-30
;;  Kyle Isom <kyle@nodality.io>


(defun check-exercise (exercise attempt expected)
   (if (equal attempt expected)
       (format t "[+] exercise ~A: success!~%" exercise)
       (format t "[!] exercise ~A: failure!~%" exercise)))



; check exercise 2
(check-exercise 2 (new-union '(a b c) '(b a d)) '(a b c d))
(check-exercise 3 (occurrences '(a b a d a c d c a))
                 '((a . 4) (c . 2) (d . 2) (b . 1)))
(check-exercise "4 (recursive)" (recur-pos+ '(7 5 1 4)) '(7 6 3 7))
(check-exercise "4 (iterative)" (iter-pos+ '(7 5 1 4)) '(7 6 3 7))
(check-exercise "4 (mapcar)" (mapcar-pos+ '(7 5 1 4)) '(7 6 3 7))