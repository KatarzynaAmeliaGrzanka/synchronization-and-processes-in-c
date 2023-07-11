# synchronization-and-processes-in-c
A program in C, which tests synchronization mechanisms and signals. Implementing a signal handler.

Using functions: fork(), wait(), signal(), kill(), sigaction().

Two program versions are implemented, without and with my keyboard interrupt signal handler. The code is in one source file and the compilation version is derived by the definition or the lack of definition of the WITH_SIGNALS preprocessor symbol.
