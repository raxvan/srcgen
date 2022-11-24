# srcgen

I find myself needing more and more a bunch of code that mostly runs at compiletime or before(generated), so i will add stuff like that here.

# What you can find

## c++ reflection code for polymorphic types

You have an array of a base interface with class instances of type (A,B,C), and you want to instance by type, with minimal code written:

ex: `get_instace_of<A>()->A_non_virtual_method()`

You can find more examples in `examples/reflection/`


# What i want to add in the future:

## Source code generator for enums, structs, etc

With utilities, enums to string, string to enum, etc


## Shader combinations generator

## Generators for communication protocols

Basically generators of abstract serializers for simple messages. No overhead, static.


