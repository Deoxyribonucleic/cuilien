env = Environment(CPPPATH=["."], CCFLAGS="-std=c99")


# Source section
vm_sources = Split("""
vm/args.c
vm/cpu.c
vm/error.c
vm/instr.c
vm/mem.c
vm/operand.c
vm/page.c
vm/util.c
vm/vector.c
vm/vm.c
""")

vmtest_sources = Split("""
vmtest/main.c
""")


# Object section
vm_objects = env.Object(source = vm_sources)
vmtest_objects = env.Object(source = vmtest_sources)


# Target section
vmtest = env.Program(target = "vmtestbin", source = [vm_objects, vmtest_objects])

