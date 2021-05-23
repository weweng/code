; emacs setup
; emacs -q -l emacs-init.el

(setq minibuffer-max-depth nil)
(setq blink-matching-paren t)
(setq scroll-step 1)
(setq inhibit-startup-screen t)

;;no back up file
(setq make-backup-files nil)

;; tab width
;;(setq tab-width 4)


;; no toolbar  display
(tool-bar-mode nil)

;;(autoload 'which-function "which-function" "" t)
(which-function-mode t)

;Replace "yes or no" with y or n
(defun yes-or-no-p (arg)
    "An alias for y-or-n-p, because I hate having to type 'yes' or 'no'."
    (y-or-n-p arg))

;; load package to display 80 column
;;(load-file "~/.emacs.d/vvb-mode.el")
;;(setq-default vvb-column 80 your-preference-here 80
;;                    vvb-sticky-p nil
;;                     vvb-permanent-p t)

(global-set-key [end] 'end-of-line)
(global-set-key [home] 'beginning-of-line)
(global-set-key [mouse-3] 'imenu)
(setq imenu-max-items 39)
(setq imenu-auto-rescan 1)

;; use nmouse wheel button
;;(require 'mwheel)
(mouse-wheel-mode t)

;; enable syntaxt color enabled upon startup
(require 'font-lock)
(global-font-lock-mode t)
(set 'font-lock-maximum-decoration t)

;; disable tool bar
(setq tool-bar-mode nil)
;; using e-shell
;;(setq eshell-prompt-function
;;  (function
;;   (lambda ()
;;     (concat
;;      (getenv "HOST")":"(eshell/pwd)
;;      (if (= (user-uid) 0) "# " "> ")))))
;;(setq eshell-prompt-regexp "^[^#$\n]*[#>] ")

;; cisco ios-c-style
(defvar cisco-ios-c-style
  '(
    (local-set-key "\C-m" 'newline-and-indent)
    (c-basic-offset                 . 4)
    (indent-tabs-mode               . t)
    (c-block-comments-indent-p      . t)
    (c-comment-only-line-offset     . 0)
    (c-echo-syntactic-information-p . nil)
    (c-tab-always-indent            . t)
    (c-hanging-comment-ender-p      . t)
    (c-recognize-knr-p              . t) ; use nil if only have ANSI prototype
    (comment-column                 . 40)
    (comment-end                    . " */")
    (comment-multi-line             . t)
    (comment-start                  . "/* ")
    (c-offsets-alist                . ((knr-argdecl-intro   . +)
                                       (case-label          . 0)
                                       (knr-argdecl         . 0)
                                       (label               . 0)
                                       (statement-case-open . +)
                                       (statement-cont      . +)
                                       (substatement-open   . 0))))
  "cisco IOS c-style for cc-mode")

;; cisco nxos-c-style
(defvar cisco-nxos-c-style
  '(
    (local-set-key "\C-m" 'newline-and-indent)
    (c-basic-offset                 . 4)
    (indent-tabs-mode               . nil)
    (c-tab-always-indent            . t)
    (c-block-comments-indent-p      . t)
    (c-comment-only-line-offset     . 0)
    (c-echo-syntactic-information-p . nil)
    (c-hanging-comment-ender-p      . t)
    (comment-column                 . 40)
    (comment-end                    . " */")
    (comment-start                  . "/* ")
    (c-offsets-alist                . ((knr-argdecl-intro   . +)
                                       (case-label          . 0)
                                       (knr-argdecl         . 0)
                                       (label               . 0)
                                       (statement-case-open . +)
                                       (statement-cont      . +)
                                       (substatement-open   . 0))))
  "cisco NXOS c-style for cc-mode")

;             (c-add-style "CISCOIOS" cisco-ios-c-style)
;             (c-add-style "CISCONXOS" cisco-nxos-c-style)

(add-hook 'c-mode-common-hook
          '(lambda ()
             (c-add-style "nxos" cisco-nxos-c-style)
             (c-add-style "ios" cisco-ios-c-style)
         (c-set-style "ios")))


;; define C-x l as goto-line
(define-key ctl-x-map "l" 'goto-line)
;;(define-key global-map "\C-xl" 'goto-line)
;;(global-set-key "\C-xl" 'goto-line)
(define-key ctl-x-map "\C-]" 'forward-sexp)
(define-key ctl-x-map "\C-[" 'backward-sexp)

;; define a function for curly bracket match seraching
(defun match-paren (arg)
   "Go to the matching parenthesis if on parenthesis otherwise insert %."
   (interactive "p")
   (cond ((looking-at "\\s\(") (forward-list 1) (backward-char 1))
         ((looking-at "\\s\)") (forward-char 1) (backward-list 1))
         (t (self-insert-command (or arg 1)))))

(global-set-key "%" 'match-paren)

;; ==============================================================
(setq transient-mark-mode t)

(defvar rename-format "%d:%s"
   "*Format string that rename-to-new-name uses to generate new name.
    The number comes first so that the least substring matches buffer names.")

(defun rename-to-new-name ()
   "Rename current buffer to new name not already taken. The new buffer names
    are in the format specified by rename-format (using old name and a number).
    This function is useful for creating multiple shell process buffers"
  (interactive)
  (let (new-name (old-name (buffer-name (current-buffer))) (i 1))
  (while (get-buffer (setq new-name (format rename-format i old-name)))
         (setq i (1+ i)))
  (rename-buffer new-name)
  (set-buffer-modified-p (buffer-modified-p)))
) ; force mode line update


(defun meshell()
(interactive)
(eshell)
(rename-to-new-name))

(defun mshell()
(interactive)
(shell)
(rename-to-new-name))

(global-set-key "\C-cy" 'meshell)

;; define hot keys
;;(global-set-key (quote [f1])  'save-buffer)
;;(global-set-key (quote [f2])  'find-file)
;(global-set-key (quote [f3])  'isearch-forward)
(global-set-key (quote [f1])  'copy-region-as-kill)
(global-set-key (quote [f2])  'yank-rectangle)
(global-set-key (quote [f3])  'kill-rectangle)

;;(global-set-key (quote [f4])  'kill-buffer)
;;(global-set-key (quote [f5])  'goto-line)
;;(global-set-key (quote [f6])  'list-buffers)
;;(global-set-key (quote [f5])  'bs-cycle-previous)
;;(global-set-key (quote [f6])  'bs-cycle-next)
;;(global-set-key (quote [f7])  'switch-to-buffer)
;;(global-set-key (quote [f7])  'speedbar)
(global-set-key (quote [f8])  'match-paren)
(global-set-key (quote [f9])  'java-mode)
(global-set-key (quote [f10])  'c-mode)
;;(global-set-key (quote [f11])  'vvb-mode)
;;(global-set-key (quote [f11])  'makefile-mode)
(global-set-key (quote [f12])  'whitespace-mode)

(custom-set-variables
  ;; custom-set-variables was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 '(c-mode-hook (quote ((lambda nil "Unbind C-c C-s so that we can use that prefix for cscope, then bind cscope keys" (local-unset-key [3 19]) (cscope-bind-keys)))))
 '(case-fold-search t)
 '(column-number-mode t)
 '(current-language-environment "Latin-1")
 '(default-input-method "latin-1-prefix")
 '(fume-index-method 3)
 '(lpr-switches (quote ("-P n2-h6-11a")))
 '(normal-erase-is-backspace t)
 '(paren-ding-unmatched t)
 '(paren-display-message (quote always))
 '(paren-highlight-offscreen t)
 '(paren-max-blinks nil)
 '(paren-mode (quote blink-paren) nil (paren))
 '(query-user-mail-address nil)
 '(scrollbars-visible-p t)
 '(show-paren-mode t nil (paren))
 '(truncate-lines t)
 '(user-mail-address "weweng@gmail.com"))
(custom-set-faces
  ;; custom-set-faces was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 '(default ((t (:stipple nil :background "white" :foreground "black" :inverse-video nil :box nil :strike-through nil :overline nil :underline nil :slant normal :weight normal :height 123 :width normal :foundry "bitstream" :family "Courier 10 Pitch"))))
 '(font-lock-comment-face ((((class color) (background light)) (:foreground "blue" :family "courier" :bold t))))
 '(font-lock-doc-string-face ((((class color) (background light)) (:foreground "green4" :bold t :italic t))))
 '(font-lock-function-name-face ((((class color) (background light)) (:foreground "red3" :family "courier" :bold t))))
 '(font-lock-keyword-face ((((class color) (background light)) (:foreground "red" :family "courier" :bold t))))
 '(font-lock-preprocessor-face ((((class color) (background light)) (:foreground "blue3" :family "courier" :bold t))))
 '(font-lock-reference-face ((((class color) (background light)) (:foreground "red3" :family "courier" :bold t))))
 '(font-lock-string-face ((((class color) (background light)) (:foreground "green4" :family "courier" :bold t))))
 '(font-lock-type-face ((((class color) (background light)) (:foreground "steelblue" :family "courier" :bold t))))
 '(font-lock-variable-name-face ((((class color) (background light)) (:foreground "magenta4" :family "courier" :bold t))))
 '(font-lock-warning-face ((((class color) (background light)) (:foreground "Red" :bold t)))))

;; ============================
;; End of Options Menu Settings
;;

(setq default-mode-line-format
   (list "-"
    'mode-line-mule-info
    'mode-line-modified
    'mode-line-frame-identification
    "%b--"
    '(line-number-mode "L%l--")
    '(column-number-mode "C%c--")
    "   %[("
    '(:eval (mode-line-mode-name))
    'mode-line-process
    'minor-mode-alist
    "%n"
    ")%]--"
    '(which-func-mode ("" which-func-format "--"))
    '(-3 "%p")
    "-%-"))
;;(setq mode-line-format default-mode-line-format)

(put 'narrow-to-region 'disabled nil)

(setq select-active-regions nil)
(setq mouse-drag-copy-region t)
;(setq mouse-drag-copy-region nil)  ; stops selection with a mouse being immediately injected to the kill ring
;(setq x-select-enable-primary nil)  ; stops killing/yanking interacting with primary X11 selection
;(setq x-select-enable-clipboard t)
(setq line-move-visual t)

