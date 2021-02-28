(*
let stretch_m (p:coded_pic) (factor:int) : coded_pic =
  failwith "Implement"

let rec map: ('a -> 'b)  -> 'a list -> 'b list = fun f l ->  
match l with  
  [] -> []  
  h::t -> f h ::  map f t


    

let segment (cx,cy) (nx,ny) =
  failwith "Implement"

    
let coverage ((start::p):coded_pic):coord list =
  failwith "Implement"


let equivalent_pics (cp1:coded_pic) (cp2:coded_pic):bool =
  failwith "Implement"


let height (p:coded_pic):int =
  failwith "Implement"


let tile ((dx,dy):coord) (p:coded_pic) : coded_pic list list =
  failwith "Implement"


let tri_aligned ((x1,y1):coord) ((x2,y2):coord) ((x3,y3):coord):bool =
  failwith "Implement"


let rec compress (p:coded_pic):coded_pic =
  failwith "Implement"

*)