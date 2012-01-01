;;; timeline.lisp
;;;;  kyle isom <kyle@nodality.io>
;;;;  code examples from rosetta code and the cl-json page
;;;;  this is an attempt to pull the twitter timeline and parse it into a list
;;;;
;;;; currently it doesn't work, my lisp fu is still weak

;; use this if you have Quicklisp (and you should)
(load #P"/Users/kyle/quicklisp/setup.lisp")
(ql:quickload "drakma")
(ql:quickload "cl-json")
(ql:quickload "babel")

;; use this if you have the libraries installed elsewhere (i.e. via
;; ASDF), this has worked on some of my systems.
(require 'drakma)
(require 'cl-json)
(require 'babel)

(json:set-decoder-simple-list-semantics)

(defparameter *public-timeline-url* 
  "http://api.twitter.com/1/statuses/public_timeline.json")

(defun get-value (key lst)
  "Extract the value of key from an assoc-list lst"
  (cdr (assoc key lst)))

(defun decode-json-bytes (arr)
  "Wrapper to read an array of bytes and decode the JSON it contains."
  (with-input-from-string 
   (s (babel:octets-to-string arr :encoding :utf-8))
   (json:decode-json s)))

(defun get-username (update)
  "Get a twitter username from a status update."
  (get-value :name (get-value :user update)))

(defun get-public-timeline ()
  "Grab the twitter public timeline."
  (decode-json-bytes (drakma:http-request *public-timeline-url*
					  :want-stream nil)))

(defun timeline-overview (timeline)
  "Run through a timeline of posts and show users and the number of characters in
the message."
  (dolist (twit timeline)
    (let ((name (get-username twit))
	  (post (get-value :text twit)))
      (format t "saw user ~A (~A chars)~%" name (length post)))))


;; let her rip
(timeline-overview (get-public-timeline))