;;; timeline.lisp
;;  kyle isom <kyle@nodality.io>
;;  code examples from rosetta code and the cl-json page
;;  this is an attempt to pull the twitter timeline and parse it into a list
;;
;; currently it doesn't work, my lisp fu is still weak

(let ((quicklisp-init (merge-pathnames "quicklisp/setup.lisp"
                                       (user-homedir-pathname))))
  (when (probe-file quicklisp-init)
    (load quicklisp-init)))

(ql:quickload "drakma")
(ql:quickload "cl-json")

(require 'drakma)
(require 'cl-json)

(defparameter *public-timeline-url* 
  "http://api.twitter.com/1/statuses/public_timeline.json")

 
(defun wget-drakma-string (url &optional (out *standard-output*))
  "Grab the body as a string, and write it to out."
  (write-string (drakma:http-request url) out))
 
(defun wget-drakma-stream (url &optional (out *standard-output*))
  "Grab the body as a stream, and write it to out."
  (loop with body = (drakma:http-request url :want-stream t)
        for line = (read-line body nil nil)
        while line do (write-line line)
        finally (close body)))

(defun get-public-timeline ()
  (let ((timeline (wget-drakma-stream *public-timeline-url*)))
    (write-string json:decode-json timeline)))


(get-public-timeline)


