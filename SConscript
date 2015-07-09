env = Environment(CPPPATH=["."], CCFLAGS="-std=c99")


# Source section
vm_sources = Split("""
vm/cpu.c
vm/error.c
vm/instr.c
vm/mem.c
vm/operand.c
vm/page.c
vm/util.c
vm/vector.c
vm/scheduler.c
""")

vmtest_sources = Split("""
vmtest/main.c
""")

disass_sources = Split("""
disass/main.c
""")


# Object section
vm_objects = env.Object(source = vm_sources)
vmtest_objects = env.Object(source = vmtest_sources)
disass_objects = env.Object(source = disass_sources)


# Target section
lib = env.Library(target = "libcuilien", source = [vm_objects])
vmtest = env.Program(target = "vmtestbin", source = [vmtest_objects], LIBS = [lib])
disass = env.Program(target = "disassemble", source = [disass_objects], LIBS = [lib, "m"])

