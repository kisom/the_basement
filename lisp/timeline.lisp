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
(ql:quickload "babel")

(require 'drakma)
(require 'cl-json)
(require 'babel)

(json:set-decoder-simple-list-semantics)

(defparameter *public-timeline-url* 
  "http://api.twitter.com/1/statuses/public_timeline.json")

(defun get-public-timeline ()
  "grab the twitter public timeline."
  (let ((tl-stream (drakma:http-request *public-timeline-url*
					 :want-stream nil)))
    (with-input-from-string (tl-string (babel:octets-to-string tl-stream
							       :encoding
							       :utf-8))
			    (json:decode-json tl-string))))

