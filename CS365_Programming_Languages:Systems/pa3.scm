;;Nagendra Upadhyay. CS365. The code below creates a few scheme functions and a merge-sort function.
(define (list-len l)
  ;;If the list is empty, return 0. Otherwise, return 1 plus the length of the rest of the list.
  (if (null? l)
      0
      (+ 1 (list-len (cdr l)))))

(define (inc-list n)
  ;;If n is 0, return an empty list. Otherwise, recursively append the result of calling `inc-list` with n-1 to a list containing n.
  (if (= n 0)
      '()
      (append (inc-list (- n 1)) (list n))))

(define (rev-list l)
  ;;If the list is empty, return an empty list. Otherwise, recursively append the result of calling `rev-list` with the rest of the list to a list containing the first element.
  (if (null? l)
      '()
      (append (rev-list (cdr l)) (list (car l)))))

(define (my-map f l)
  ;;If l is empty, return an empty list. Otherwise, cons the result of applying f to the first element of l to the result of calling `my-map` with f and the rest of the list.
  (if (null? l)
      '()
      (cons (apply f (list (car l))) (my-map f (cdr l)))))

(define (merge-sort l)
  ;;Define a helper function `split` that recursively splits a list in half until each half contains only one element
  (define (split l)
    (define (split-rec pair)
      ;;Get the front half and the back half of the list
      (let ((front (car pair)) (back (cdr pair)))
        ;;If the front half is longer than or equal to the back half, return the pair
        (if (>= (length front) (length back))
            pair
            ;;Otherwise, append the first element of the back half to the front half and recurse on the resulting pair
            (split-rec (cons (append front (list (car back))) (cdr back))))))
    ;;Call `split-rec` with a pair containing a list of the first element of the input list and the rest of the input list
    (split-rec (cons (list (car l)) (cdr l))))
  
  ;;Define a helper function `merge` that merges two sorted lists into a single sorted list
  (define (merge left right)
    ;;If `left` is empty, return `right`
    (cond ((null? left) right)
          ;;If `right` is empty, return `left`
          ((null? right) left)
          ;;If the first element of `left` is less than the first element of `right`, cons the first element of `left` onto the result of merging the rest of `left` with `right`
          ((< (car left) (car right))
           (cons (car left) (merge (cdr left) right)))
          ;;Otherwise, cons the first element of `right` onto the result of merging `left` with the rest of `right`
          (else (cons (car right) (merge left (cdr right))))))
  
  ;;If the input list is empty or contains only one element, return the input list
  (if (or (null? l) (null? (cdr l)))
      l
      ;;Otherwise, split the list in half, sort the left half and right half recursively, and then merge the sorted halves
      (let* ((halves (split l))
             (left (merge-sort (car halves)))
             (right (merge-sort (cdr halves))))
        (merge left right))))
