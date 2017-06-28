# NESL to Annotated C compiler

## Build

    $ make all

## Use
    
    $ ./NESL  <neslfile>.nesl [-sqc][-pfc][-rev]

* -pfc for PartialFlattening C.
* -sqc for Sequential C.
* -rev for reverse output NESL.

## Compile sqc

    $ gcc output/<neslfile>.c 

## Execution

    $ ./a.out

## Quicksort demo

    $ chmod +x demo_SEQ.sh
    $ ./demo_SEQ.sh
