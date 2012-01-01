(load "timeline.lisp")

;; let her rip
(let ((timeline (get-public-timeline)))  ; only grab status once to minimise
  (timeline-overview timeline)           ; calls against the API (for rate
  (show-updates timeline))               ; limiting purposes)
