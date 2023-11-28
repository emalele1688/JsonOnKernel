# JsonOnKernel

This is a project to support JSON standard text on the Linux Kernel.

## The KJSON library

KJSON (JsonOnKernel) library as an internal parser to create the JSON data structure from a JSON standard text. Or you can dump a JSON text from your data structure.

You can link the library direclty on you kernel module code, or use the prebuilt module with the makefile provvided.

## JSON object supported

* Integer
* String
* Annidate JSON
* Integer array
* String array
* Annidate JSON array

## Building

You can build the kjson library module and used the Module.symvers generated on your makefile. Or you can put the sources and headers file directly on your project.

## Example

Visit the `samples` subdirectory.
