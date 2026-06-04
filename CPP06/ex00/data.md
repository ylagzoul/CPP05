# Type Conversion in C++ & x86 — Complete Guide

---

## 1. What Is static_cast?

`static_cast` is a compile-time type conversion operator in C++. It is not a system call — it never touches the OS kernel. It is purely a compiler directive that tells the compiler how to reinterpret or convert a value from one type to another.

```cpp
int x = 42;
double d = static_cast<double>(x);  // No syscall. Pure compiler work.
```

---

## 2. The Four Kinds of static_cast

### 2a. Numeric Conversions

```cpp
int i = 100;
double d = static_cast<double>(i);
```

The compiler emits a CPU instruction like `cvtsi2sd` (on x86) to convert an integer register to a floating-point register. No memory allocation, no OS involvement, zero runtime overhead beyond the instruction itself.

### 2b. Pointer Upcasting / Downcasting (Inheritance)

```cpp
class Base { virtual void foo() {} };
class Derived : public Base { void bar() {} };

Derived* d = new Derived();
Base* b = static_cast<Base*>(d);        // upcast — always safe
Derived* d2 = static_cast<Derived*>(b); // downcast — you assert safety
```

For upcasting: the compiler may adjust the pointer value by an offset (in multiple inheritance). For downcasting: no runtime check — you are telling the compiler "trust me, this is a Derived*". Wrong assumption = undefined behavior. Still no syscall — the compiler just adds/subtracts a compile-time constant from the pointer.

**Memory Layout (multiple inheritance example):**

```
┌──────────────────────────┐
│  Base subobject (offset 0)│  ← Base* points here
├──────────────────────────┤
│  Derived members         │  ← Derived* points to start
└──────────────────────────┘

static_cast<Base*>(derived_ptr) = derived_ptr + sizeof(Base_offset)
```

### 2c. void* Conversions

```cpp
void* raw = malloc(100);
int* ip = static_cast<int*>(raw);  // tell compiler: treat this memory as int*
```

The pointer value is unchanged — it's the same address. The compiler just changes how it emits access instructions (byte width, alignment assumptions).

### 2d. enum ↔ Integer

```cpp
enum Color { Red = 0, Green = 1, Blue = 2 };
int n = static_cast<int>(Color::Green);  // → 1
Color c = static_cast<Color>(2);         // → Blue
```

Purely compile-time integer aliasing. Zero overhead.

---

## 3. static_cast vs. Other Casts

| Cast | When | Compile-time? | Runtime check? | Safe? |
|------|------|---------------|----------------|-------|
| `static_cast` | Related types, numeric, pointer hierarchy | ✅ Yes | ❌ No | ⚠️ Mostly |
| `dynamic_cast` | Polymorphic downcasting | Partial | ✅ Yes (RTTI) | ✅ Yes |
| `reinterpret_cast` | Raw bit reinterpretation | ✅ Yes | ❌ No | ❌ Dangerous |
| `const_cast` | Add/remove const | ✅ Yes | ❌ No | ⚠️ Careful |
| C-style `(T)x` | Any of the above | Varies | Varies | ❌ Unpredictable |

### static_cast vs dynamic_cast — The Key Difference

```cpp
Base* b = new Base();  // NOT a Derived!

// static_cast — blind trust, no check:
Derived* d1 = static_cast<Derived*>(b);  // compiles fine → UB at runtime!

// dynamic_cast — uses RTTI, checks vtable at runtime:
Derived* d2 = dynamic_cast<Derived*>(b); // returns nullptr safely
```

`dynamic_cast` does have runtime overhead — it walks the RTTI (Run-Time Type Information) tables the compiler embeds in your binary. Still no syscall, but it's not free.

---

## 4. What Happens at the OS/Kernel Level?

Absolutely nothing. Here's the full execution chain:

```
static_cast<T>(x)
       │
       ▼
  [Compiler validates legality at compile time]
       │
       ▼
  [Emits CPU instruction: convert/adjust/reinterpret]
       │
       ▼
  [CPU executes in a single clock cycle or less]
       │
       ▼
  [OS/Kernel: never woken up, never consulted]
```

| Layer | What Happens |
|-------|-------------|
| C++ Source | You write `static_cast<T>(x)` |
| Compiler (Clang/GCC) | Validates the cast at compile time, emits appropriate machine instructions |
| CPU | Executes a register conversion instruction (e.g. `cvtsi2sd`, `movsx`, `lea`) |
| OS/Kernel | 💤 Not involved at all |

The OS only gets involved when you do things like `malloc` (→ `brk`/`mmap` syscall), file I/O, threading, etc. A type cast is pure ALU/register work.

---

## 5. The cvtsi2sd Instruction — Integer → Double

`cvtsi2sd` is the SSE2 instruction the compiler emits for `int → double` conversion.

### Anatomy of the Name

| Part | Meaning |
|------|---------|
| `cvt` | Convert |
| `si` | Signed Integer |
| `2` | To |
| `sd` | Scalar Double-precision |

### What It Does

```asm
cvtsi2sd xmm0, rax
```

- Reads a signed integer from a general-purpose register (e.g. `rax`, `eax`)
- Converts it to a 64-bit double
- Writes the result into the lower 64 bits of an XMM register (e.g. `xmm0`)

In C terms:

```c
int i = 42;
double d = (double) i;   // compiler emits cvtsi2sd
```

### Why Integers and Floats Need a Special Instruction

You can't just move the bits — the formats are completely different:

```
Integer 42:   00000000 00000000 00000000 00101010  (two's complement)

Double 42.0:  0 10000000100 0101000...000  (sign | exponent | mantissa)
```

The CPU must actually reformat the value, which `cvtsi2sd` does in a single clock cycle.

### Compiler Example

```c
double square(int x) {
    return (double)x * x;
}
```

Compiles to roughly:

```asm
cvtsi2sd  xmm0, edi    ; convert int arg to double
cvtsi2sd  xmm1, edi    ; convert again
mulsd     xmm0, xmm1   ; multiply two doubles
ret
```

### The Full Instruction Family

| Instruction | Converts… | To… |
|-------------|-----------|-----|
| `cvtsi2sd` | signed int | scalar double |
| `cvtsi2ss` | signed int | scalar single (float) |
| `cvtsd2si` | scalar double | signed int |
| `cvtss2si` | scalar single | signed int |
| `cvttsd2si` | scalar double | signed int (truncate) |
| `cvtdq2ps` | packed ints | packed singles |

---

## 6. Why int → double Is Truly "Zero-Cost"

### No Memory Allocation

The conversion happens entirely inside CPU registers — nothing touches RAM:

```
General-Purpose Registers        XMM Registers (SSE)
┌─────────────────────┐          ┌──────────────────────────┐
│  rax  │  42 (int)   │ ──────►  │  xmm0  │  42.0 (double) │
└─────────────────────┘          └──────────────────────────┘
         source                           destination
```

No heap allocation. No stack frame. No `malloc()`. No `new`. Compare to `std::to_string(42)` which does allocate a string on the heap — `cvtsi2sd` allocates nothing because registers are not "memory" in the allocator sense.

### No OS Involvement

A normal allocation path:

```
your code → malloc() → heap allocator → brk()/mmap() → OS kernel
                                                            │
                                                       page table update
                                                       memory mapping
                                                       context switch?
```

`cvtsi2sd` path:

```
your code → cvtsi2sd   ← that's it. never leaves userspace.
```

No syscall. No ring-0 transition. No kernel scheduler involvement.

### No Hidden Work Triggered

| Operation | Looks cheap? | Hidden cost |
|-----------|-------------|-------------|
| `(double)i` → `cvtsi2sd` | ✅ actually cheap | none |
| `(double*)malloc(8)` | ❌ expensive | syscall, lock, bookkeeping |
| Accessing a new memory page | ❌ expensive | page fault, OS handler |
| Virtual function call | ❌ moderate | vtable pointer chase, branch mispredict |
| `std::any` cast | ❌ moderate | `type_info` comparison, possible alloc |

### What "The Instruction Itself" Actually Costs

```
cvtsi2sd xmm0, rax  (Intel Skylake / AMD Zen 3)
├── Latency:    ~4 cycles        (result ready 4 cycles later)
├── Throughput: 1 per cycle      (CPU can start a new one every cycle)
└── Execution units: 1          (uses FP conversion unit)

For context:
├── Integer add (add rax, rbx):  1 cycle latency
├── Memory load (mov rax, [p]):  4 cycles  (L1 cache hit)
└── Division (div):              20–90 cycles latency
```

### Complete List of What Does NOT Happen

```
✗  No heap allocation       (no malloc/free)
✗  No stack allocation      (no push/pop/sub rsp)
✗  No function call         (no call/ret, no ABI overhead)
✗  No system call           (no int 0x80, no syscall instruction)
✗  No cache miss            (registers have no cache)
✗  No branch                (no conditional logic)
✗  No lock / mutex          (registers are per-core, not shared)
✗  No garbage collection    (no GC write barrier)
✗  No exception path        (no try/catch machinery)
✗  No RTTI                  (no type_info lookup)

✓  One instruction.
✓  One execution unit.
✓  ~4 cycles.
✓  Done.
```

---

## 7. The Reverse Direction — double → int with cvttsd2si

Going the other direction uses a different instruction:

```c
double i = 42.0;
int d = (int) i;
```

```asm
cvttsd2si  eax, xmm0    ; Convert with Truncation: Scalar Double → Signed Int
```

Notice the extra `t` — `cvtt` instead of `cvt`. That `t` matters a lot.

### Truncation vs. Rounding

| Instruction | Mode | 3.9 → | -3.9 → |
|-------------|------|--------|---------|
| `cvtsd2si` | Rounded (nearest) | 4 | -4 |
| `cvttsd2si` | Truncated (toward zero) | 3 | -3 |

C's `(int)` cast is defined as truncation, so the compiler always picks `cvttsd2si`.

```c
(int)  3.9  →   3    (drops the .9)
(int) -3.9  →  -3    (drops the .9, moves toward zero)
(int)  3.0  →   3    (exact, no issue)
```

It is not rounding — it always chops toward zero.

### Why This Direction Is Dangerous

`int → double` is always safe: a 32-bit int has at most 31 significant bits, and a double has 52 bits of mantissa — it can represent every int exactly.

`double → int` is the dangerous direction:

```
double has range: ±1.8 × 10³⁰⁸
int    has range: −2,147,483,648  to  2,147,483,647
```

Three bad things can happen:

```c
(int) 42.0         →  42          ✅ fine
(int) 42.9         →  42          ✅ fine (truncation)
(int) 2147483648.0 →  ???         ❌ overflow  (too big for int)
(int) 1e300        →  ???         ❌ overflow  (way too big)
(int) NaN          →  ???         ❌ undefined
(int) Infinity     →  ???         ❌ undefined
```

### The "Integer Indefinite" Sentinel — What the CPU Does on Overflow

The x86 CPU doesn't crash or throw — it returns a special sentinel value:

```
0x80000000             = −2,147,483,648  (32-bit int)
0x8000000000000000     (64-bit long)
```

```c
double huge = 1e300;
int d = (int) huge;
printf("%d\n", d);   // prints -2147483648  ← silent corruption, no crash
```

No exception. No signal. Silently wrong.

---

## 8. Deep Dive: cvttsd2si on 21474836495.2

This walks through every bit of a real overflow case.

### Step 1 — Represent 21474836495 in Binary

```
2^34 = 17,179,869,184
21,474,836,495 − 17,179,869,184 = 4,294,967,311

2^32 = 4,294,967,296
4,294,967,311 − 4,294,967,296 = 15  =  2^3 + 2^2 + 2^1 + 2^0

So: 21,474,836,495 = 2^34 + 2^32 + 2^3 + 2^2 + 2^1 + 2^0
Binary: 1_0100_0000_0000_0000_0000_0000_0000_1111
```

### Step 2 — Normalize to IEEE 754 Scientific Binary

```
1_0100_0000_0000_0000_0000_0000_0000_1111.0010...
= 1.0100000000000000000000000000001111_0010... × 2^34
         ↑
    drop the leading 1 (implicit in IEEE 754)
```

The `.2` fractional part (0.2 in binary = 0.0011 0011 0011... repeating) appends to the mantissa after the integer bits.

```
52-bit mantissa:
0100 0000 0000 0000 0000 0000 0000 1111 0011 0011 0011 0011
|_________________52 bits___________________________________|
```

### Step 3 — Encode the Exponent

```
Actual exponent = 34
IEEE 754 bias   = 1023
Stored exponent = 34 + 1023 = 1057 = 100_0010_0001 in binary
```

### Step 4 — Full IEEE 754 Bit Pattern

```
S  Exponent (11 bits)  Mantissa (52 bits)
──────────────────────────────────────────────────────────────
0  100 0010 0001       0100 0000 0000 0000 0000 0000 0000 1111 0011 0011 0011 0011

Hex: 0x4220_0000_0000_0F33
```

### Step 5 — cvttsd2si Executes Step by Step

```
1. Extract sign bit        →  0  (positive)
2. Decode exponent         →  1057 − 1023 = 34  →  scaled by 2^34
3. Restore implicit 1      →  1.0100000000000000000000000000001111 0011...
4. Shift left by 34        →  move decimal point 34 places right
   integer part: 1_0100_0000_0000_0000_0000_0000_0000_1111 = 21,474,836,495 ✓
5. Truncate fractional bits →  .0011 0011...  DROPPED
6. Check range              →  21,474,836,495 > 2,147,483,647 (INT_MAX) ❌ OVERFLOW
7. Write sentinel value     →  eax = 0x80000000 = −2,147,483,648
```

```
                          ┌──────────────┐
xmm0  [ 21474836495.2 ] ─►│  cvttsd2si   │──►  eax = 0x80000000 = −2,147,483,648
                          └──────────────┘               ≠ 21,474,836,495
                                                         SILENT CORRUPTION ⚠️
```

Note: the `.2` fractional part is irrelevant here — the integer part alone already overflows. The CPU detects this during range check and returns the Integer Indefinite sentinel with no exception, no signal, no warning.

### Fix: Use long Instead

```c
long d = (long) i;   // compiler emits:  cvttsd2si rax, xmm0  (64-bit)
```

```
64-bit int range: −9,223,372,036,854,775,808  to  9,223,372,036,854,775,807
21,474,836,495 fits comfortably ✓
rax = 21,474,836,495   ← correct, truncated (drops the .2)
```

---

## 9. Safe Conversion in Practice

If you care about correctness, you need to guard it yourself — the CPU will not protect you:

```c
// C — manual range check
if (i >= INT_MIN && i <= INT_MAX && !isnan(i)) {
    d = (int) i;
}

// C++ — safer cast (throws on failure)
d = boost::numeric_cast<int>(i);  // throws bad_numeric_cast if out of range
```# Type Conversion in C++ & x86 — Complete Guide

---

## 1. What Is static_cast?

`static_cast` is a compile-time type conversion operator in C++. It is not a system call — it never touches the OS kernel. It is purely a compiler directive that tells the compiler how to reinterpret or convert a value from one type to another.

```cpp
int x = 42;
double d = static_cast<double>(x);  // No syscall. Pure compiler work.
```

---

## 2. The Four Kinds of static_cast

### 2a. Numeric Conversions

```cpp
int i = 100;
double d = static_cast<double>(i);
```

The compiler emits a CPU instruction like `cvtsi2sd` (on x86) to convert an integer register to a floating-point register. No memory allocation, no OS involvement, zero runtime overhead beyond the instruction itself.

### 2b. Pointer Upcasting / Downcasting (Inheritance)

```cpp
class Base { virtual void foo() {} };
class Derived : public Base { void bar() {} };

Derived* d = new Derived();
Base* b = static_cast<Base*>(d);        // upcast — always safe
Derived* d2 = static_cast<Derived*>(b); // downcast — you assert safety
```

For upcasting: the compiler may adjust the pointer value by an offset (in multiple inheritance). For downcasting: no runtime check — you are telling the compiler "trust me, this is a Derived*". Wrong assumption = undefined behavior. Still no syscall — the compiler just adds/subtracts a compile-time constant from the pointer.

**Memory Layout (multiple inheritance example):**

```
┌──────────────────────────┐
│  Base subobject (offset 0)│  ← Base* points here
├──────────────────────────┤
│  Derived members         │  ← Derived* points to start
└──────────────────────────┘

static_cast<Base*>(derived_ptr) = derived_ptr + sizeof(Base_offset)
```

### 2c. void* Conversions

```cpp
void* raw = malloc(100);
int* ip = static_cast<int*>(raw);  // tell compiler: treat this memory as int*
```

The pointer value is unchanged — it's the same address. The compiler just changes how it emits access instructions (byte width, alignment assumptions).

### 2d. enum ↔ Integer

```cpp
enum Color { Red = 0, Green = 1, Blue = 2 };
int n = static_cast<int>(Color::Green);  // → 1
Color c = static_cast<Color>(2);         // → Blue
```

Purely compile-time integer aliasing. Zero overhead.

---

## 3. static_cast vs. Other Casts

| Cast | When | Compile-time? | Runtime check? | Safe? |
|------|------|---------------|----------------|-------|
| `static_cast` | Related types, numeric, pointer hierarchy | ✅ Yes | ❌ No | ⚠️ Mostly |
| `dynamic_cast` | Polymorphic downcasting | Partial | ✅ Yes (RTTI) | ✅ Yes |
| `reinterpret_cast` | Raw bit reinterpretation | ✅ Yes | ❌ No | ❌ Dangerous |
| `const_cast` | Add/remove const | ✅ Yes | ❌ No | ⚠️ Careful |
| C-style `(T)x` | Any of the above | Varies | Varies | ❌ Unpredictable |

### static_cast vs dynamic_cast — The Key Difference

```cpp
Base* b = new Base();  // NOT a Derived!

// static_cast — blind trust, no check:
Derived* d1 = static_cast<Derived*>(b);  // compiles fine → UB at runtime!

// dynamic_cast — uses RTTI, checks vtable at runtime:
Derived* d2 = dynamic_cast<Derived*>(b); // returns nullptr safely
```

`dynamic_cast` does have runtime overhead — it walks the RTTI (Run-Time Type Information) tables the compiler embeds in your binary. Still no syscall, but it's not free.

---

## 4. What Happens at the OS/Kernel Level?

Absolutely nothing. Here's the full execution chain:

```
static_cast<T>(x)
       │
       ▼
  [Compiler validates legality at compile time]
       │
       ▼
  [Emits CPU instruction: convert/adjust/reinterpret]
       │
       ▼
  [CPU executes in a single clock cycle or less]
       │
       ▼
  [OS/Kernel: never woken up, never consulted]
```

| Layer | What Happens |
|-------|-------------|
| C++ Source | You write `static_cast<T>(x)` |
| Compiler (Clang/GCC) | Validates the cast at compile time, emits appropriate machine instructions |
| CPU | Executes a register conversion instruction (e.g. `cvtsi2sd`, `movsx`, `lea`) |
| OS/Kernel | 💤 Not involved at all |

The OS only gets involved when you do things like `malloc` (→ `brk`/`mmap` syscall), file I/O, threading, etc. A type cast is pure ALU/register work.

---

## 5. The cvtsi2sd Instruction — Integer → Double

`cvtsi2sd` is the SSE2 instruction the compiler emits for `int → double` conversion.

### Anatomy of the Name

| Part | Meaning |
|------|---------|
| `cvt` | Convert |
| `si` | Signed Integer |
| `2` | To |
| `sd` | Scalar Double-precision |

### What It Does

```asm
cvtsi2sd xmm0, rax
```

- Reads a signed integer from a general-purpose register (e.g. `rax`, `eax`)
- Converts it to a 64-bit double
- Writes the result into the lower 64 bits of an XMM register (e.g. `xmm0`)

In C terms:

```c
int i = 42;
double d = (double) i;   // compiler emits cvtsi2sd
```

### Why Integers and Floats Need a Special Instruction

You can't just move the bits — the formats are completely different:

```
Integer 42:   00000000 00000000 00000000 00101010  (two's complement)

Double 42.0:  0 10000000100 0101000...000  (sign | exponent | mantissa)
```

The CPU must actually reformat the value, which `cvtsi2sd` does in a single clock cycle.

### Compiler Example

```c
double square(int x) {
    return (double)x * x;
}
```

Compiles to roughly:

```asm
cvtsi2sd  xmm0, edi    ; convert int arg to double
cvtsi2sd  xmm1, edi    ; convert again
mulsd     xmm0, xmm1   ; multiply two doubles
ret
```

### The Full Instruction Family

| Instruction | Converts… | To… |
|-------------|-----------|-----|
| `cvtsi2sd` | signed int | scalar double |
| `cvtsi2ss` | signed int | scalar single (float) |
| `cvtsd2si` | scalar double | signed int |
| `cvtss2si` | scalar single | signed int |
| `cvttsd2si` | scalar double | signed int (truncate) |
| `cvtdq2ps` | packed ints | packed singles |

---

## 6. Why int → double Is Truly "Zero-Cost"

### No Memory Allocation

The conversion happens entirely inside CPU registers — nothing touches RAM:

```
General-Purpose Registers        XMM Registers (SSE)
┌─────────────────────┐          ┌──────────────────────────┐
│  rax  │  42 (int)   │ ──────►  │  xmm0  │  42.0 (double) │
└─────────────────────┘          └──────────────────────────┘
         source                           destination
```

No heap allocation. No stack frame. No `malloc()`. No `new`. Compare to `std::to_string(42)` which does allocate a string on the heap — `cvtsi2sd` allocates nothing because registers are not "memory" in the allocator sense.

### No OS Involvement

A normal allocation path:

```
your code → malloc() → heap allocator → brk()/mmap() → OS kernel
                                                            │
                                                       page table update
                                                       memory mapping
                                                       context switch?
```

`cvtsi2sd` path:

```
your code → cvtsi2sd   ← that's it. never leaves userspace.
```

No syscall. No ring-0 transition. No kernel scheduler involvement.

### No Hidden Work Triggered

| Operation | Looks cheap? | Hidden cost |
|-----------|-------------|-------------|
| `(double)i` → `cvtsi2sd` | ✅ actually cheap | none |
| `(double*)malloc(8)` | ❌ expensive | syscall, lock, bookkeeping |
| Accessing a new memory page | ❌ expensive | page fault, OS handler |
| Virtual function call | ❌ moderate | vtable pointer chase, branch mispredict |
| `std::any` cast | ❌ moderate | `type_info` comparison, possible alloc |

### What "The Instruction Itself" Actually Costs

```
cvtsi2sd xmm0, rax  (Intel Skylake / AMD Zen 3)
├── Latency:    ~4 cycles        (result ready 4 cycles later)
├── Throughput: 1 per cycle      (CPU can start a new one every cycle)
└── Execution units: 1          (uses FP conversion unit)

For context:
├── Integer add (add rax, rbx):  1 cycle latency
├── Memory load (mov rax, [p]):  4 cycles  (L1 cache hit)
└── Division (div):              20–90 cycles latency
```

### Complete List of What Does NOT Happen

```
✗  No heap allocation       (no malloc/free)
✗  No stack allocation      (no push/pop/sub rsp)
✗  No function call         (no call/ret, no ABI overhead)
✗  No system call           (no int 0x80, no syscall instruction)
✗  No cache miss            (registers have no cache)
✗  No branch                (no conditional logic)
✗  No lock / mutex          (registers are per-core, not shared)
✗  No garbage collection    (no GC write barrier)
✗  No exception path        (no try/catch machinery)
✗  No RTTI                  (no type_info lookup)

✓  One instruction.
✓  One execution unit.
✓  ~4 cycles.
✓  Done.
```

---

## 7. The Reverse Direction — double → int with cvttsd2si

Going the other direction uses a different instruction:

```c
double i = 42.0;
int d = (int) i;
```

```asm
cvttsd2si  eax, xmm0    ; Convert with Truncation: Scalar Double → Signed Int
```

Notice the extra `t` — `cvtt` instead of `cvt`. That `t` matters a lot.

### Truncation vs. Rounding

| Instruction | Mode | 3.9 → | -3.9 → |
|-------------|------|--------|---------|
| `cvtsd2si` | Rounded (nearest) | 4 | -4 |
| `cvttsd2si` | Truncated (toward zero) | 3 | -3 |

C's `(int)` cast is defined as truncation, so the compiler always picks `cvttsd2si`.

```c
(int)  3.9  →   3    (drops the .9)
(int) -3.9  →  -3    (drops the .9, moves toward zero)
(int)  3.0  →   3    (exact, no issue)
```

It is not rounding — it always chops toward zero.

### Why This Direction Is Dangerous

`int → double` is always safe: a 32-bit int has at most 31 significant bits, and a double has 52 bits of mantissa — it can represent every int exactly.

`double → int` is the dangerous direction:

```
double has range: ±1.8 × 10³⁰⁸
int    has range: −2,147,483,648  to  2,147,483,647
```

Three bad things can happen:

```c
(int) 42.0         →  42          ✅ fine
(int) 42.9         →  42          ✅ fine (truncation)
(int) 2147483648.0 →  ???         ❌ overflow  (too big for int)
(int) 1e300        →  ???         ❌ overflow  (way too big)
(int) NaN          →  ???         ❌ undefined
(int) Infinity     →  ???         ❌ undefined
```

### The "Integer Indefinite" Sentinel — What the CPU Does on Overflow

The x86 CPU doesn't crash or throw — it returns a special sentinel value:

```
0x80000000             = −2,147,483,648  (32-bit int)
0x8000000000000000     (64-bit long)
```

```c
double huge = 1e300;
int d = (int) huge;
printf("%d\n", d);   // prints -2147483648  ← silent corruption, no crash
```

No exception. No signal. Silently wrong.

---

## 8. Deep Dive: cvttsd2si on 21474836495.2

This walks through every bit of a real overflow case.

### Step 1 — Represent 21474836495 in Binary

```
2^34 = 17,179,869,184
21,474,836,495 − 17,179,869,184 = 4,294,967,311

2^32 = 4,294,967,296
4,294,967,311 − 4,294,967,296 = 15  =  2^3 + 2^2 + 2^1 + 2^0

So: 21,474,836,495 = 2^34 + 2^32 + 2^3 + 2^2 + 2^1 + 2^0
Binary: 1_0100_0000_0000_0000_0000_0000_0000_1111
```

### Step 2 — Normalize to IEEE 754 Scientific Binary

```
1_0100_0000_0000_0000_0000_0000_0000_1111.0010...
= 1.0100000000000000000000000000001111_0010... × 2^34
         ↑
    drop the leading 1 (implicit in IEEE 754)
```

The `.2` fractional part (0.2 in binary = 0.0011 0011 0011... repeating) appends to the mantissa after the integer bits.

```
52-bit mantissa:
0100 0000 0000 0000 0000 0000 0000 1111 0011 0011 0011 0011
|_________________52 bits___________________________________|
```

### Step 3 — Encode the Exponent

```
Actual exponent = 34
IEEE 754 bias   = 1023
Stored exponent = 34 + 1023 = 1057 = 100_0010_0001 in binary
```

### Step 4 — Full IEEE 754 Bit Pattern

```
S  Exponent (11 bits)  Mantissa (52 bits)
──────────────────────────────────────────────────────────────
0  100 0010 0001       0100 0000 0000 0000 0000 0000 0000 1111 0011 0011 0011 0011

Hex: 0x4220_0000_0000_0F33
```

### Step 5 — cvttsd2si Executes Step by Step

```
1. Extract sign bit        →  0  (positive)
2. Decode exponent         →  1057 − 1023 = 34  →  scaled by 2^34
3. Restore implicit 1      →  1.0100000000000000000000000000001111 0011...
4. Shift left by 34        →  move decimal point 34 places right
   integer part: 1_0100_0000_0000_0000_0000_0000_0000_1111 = 21,474,836,495 ✓
5. Truncate fractional bits →  .0011 0011...  DROPPED
6. Check range              →  21,474,836,495 > 2,147,483,647 (INT_MAX) ❌ OVERFLOW
7. Write sentinel value     →  eax = 0x80000000 = −2,147,483,648
```

```
                          ┌──────────────┐
xmm0  [ 21474836495.2 ] ─►│  cvttsd2si   │──►  eax = 0x80000000 = −2,147,483,648
                          └──────────────┘               ≠ 21,474,836,495
                                                         SILENT CORRUPTION ⚠️
```

Note: the `.2` fractional part is irrelevant here — the integer part alone already overflows. The CPU detects this during range check and returns the Integer Indefinite sentinel with no exception, no signal, no warning.

### Fix: Use long Instead

```c
long d = (long) i;   // compiler emits:  cvttsd2si rax, xmm0  (64-bit)
```

```
64-bit int range: −9,223,372,036,854,775,808  to  9,223,372,036,854,775,807
21,474,836,495 fits comfortably ✓
rax = 21,474,836,495   ← correct, truncated (drops the .2)
```

---

## 9. Safe Conversion in Practice

If you care about correctness, you need to guard it yourself — the CPU will not protect you:

```c
// C — manual range check
if (i >= INT_MIN && i <= INT_MAX && !isnan(i)) {
    d = (int) i;
}

// C++ — safer cast (throws on failure)
d = boost::numeric_cast<int>(i);  // throws bad_numeric_cast if out of range
```# Type Conversion in C++ & x86 — Complete Guide

---

## 1. What Is static_cast?

`static_cast` is a compile-time type conversion operator in C++. It is not a system call — it never touches the OS kernel. It is purely a compiler directive that tells the compiler how to reinterpret or convert a value from one type to another.

```cpp
int x = 42;
double d = static_cast<double>(x);  // No syscall. Pure compiler work.
```

---

## 2. The Four Kinds of static_cast

### 2a. Numeric Conversions

```cpp
int i = 100;
double d = static_cast<double>(i);
```

The compiler emits a CPU instruction like `cvtsi2sd` (on x86) to convert an integer register to a floating-point register. No memory allocation, no OS involvement, zero runtime overhead beyond the instruction itself.

### 2b. Pointer Upcasting / Downcasting (Inheritance)

```cpp
class Base { virtual void foo() {} };
class Derived : public Base { void bar() {} };

Derived* d = new Derived();
Base* b = static_cast<Base*>(d);        // upcast — always safe
Derived* d2 = static_cast<Derived*>(b); // downcast — you assert safety
```

For upcasting: the compiler may adjust the pointer value by an offset (in multiple inheritance). For downcasting: no runtime check — you are telling the compiler "trust me, this is a Derived*". Wrong assumption = undefined behavior. Still no syscall — the compiler just adds/subtracts a compile-time constant from the pointer.

**Memory Layout (multiple inheritance example):**

```
┌──────────────────────────┐
│  Base subobject (offset 0)│  ← Base* points here
├──────────────────────────┤
│  Derived members         │  ← Derived* points to start
└──────────────────────────┘

static_cast<Base*>(derived_ptr) = derived_ptr + sizeof(Base_offset)
```

### 2c. void* Conversions

```cpp
void* raw = malloc(100);
int* ip = static_cast<int*>(raw);  // tell compiler: treat this memory as int*
```

The pointer value is unchanged — it's the same address. The compiler just changes how it emits access instructions (byte width, alignment assumptions).

### 2d. enum ↔ Integer

```cpp
enum Color { Red = 0, Green = 1, Blue = 2 };
int n = static_cast<int>(Color::Green);  // → 1
Color c = static_cast<Color>(2);         // → Blue
```

Purely compile-time integer aliasing. Zero overhead.

---

## 3. static_cast vs. Other Casts

| Cast | When | Compile-time? | Runtime check? | Safe? |
|------|------|---------------|----------------|-------|
| `static_cast` | Related types, numeric, pointer hierarchy | ✅ Yes | ❌ No | ⚠️ Mostly |
| `dynamic_cast` | Polymorphic downcasting | Partial | ✅ Yes (RTTI) | ✅ Yes |
| `reinterpret_cast` | Raw bit reinterpretation | ✅ Yes | ❌ No | ❌ Dangerous |
| `const_cast` | Add/remove const | ✅ Yes | ❌ No | ⚠️ Careful |
| C-style `(T)x` | Any of the above | Varies | Varies | ❌ Unpredictable |

### static_cast vs dynamic_cast — The Key Difference

```cpp
Base* b = new Base();  // NOT a Derived!

// static_cast — blind trust, no check:
Derived* d1 = static_cast<Derived*>(b);  // compiles fine → UB at runtime!

// dynamic_cast — uses RTTI, checks vtable at runtime:
Derived* d2 = dynamic_cast<Derived*>(b); // returns nullptr safely
```

`dynamic_cast` does have runtime overhead — it walks the RTTI (Run-Time Type Information) tables the compiler embeds in your binary. Still no syscall, but it's not free.

---

## 4. What Happens at the OS/Kernel Level?

Absolutely nothing. Here's the full execution chain:

```
static_cast<T>(x)
       │
       ▼
  [Compiler validates legality at compile time]
       │
       ▼
  [Emits CPU instruction: convert/adjust/reinterpret]
       │
       ▼
  [CPU executes in a single clock cycle or less]
       │
       ▼
  [OS/Kernel: never woken up, never consulted]
```

| Layer | What Happens |
|-------|-------------|
| C++ Source | You write `static_cast<T>(x)` |
| Compiler (Clang/GCC) | Validates the cast at compile time, emits appropriate machine instructions |
| CPU | Executes a register conversion instruction (e.g. `cvtsi2sd`, `movsx`, `lea`) |
| OS/Kernel | 💤 Not involved at all |

The OS only gets involved when you do things like `malloc` (→ `brk`/`mmap` syscall), file I/O, threading, etc. A type cast is pure ALU/register work.

---

## 5. The cvtsi2sd Instruction — Integer → Double

`cvtsi2sd` is the SSE2 instruction the compiler emits for `int → double` conversion.

### Anatomy of the Name

| Part | Meaning |
|------|---------|
| `cvt` | Convert |
| `si` | Signed Integer |
| `2` | To |
| `sd` | Scalar Double-precision |

### What It Does

```asm
cvtsi2sd xmm0, rax
```

- Reads a signed integer from a general-purpose register (e.g. `rax`, `eax`)
- Converts it to a 64-bit double
- Writes the result into the lower 64 bits of an XMM register (e.g. `xmm0`)

In C terms:

```c
int i = 42;
double d = (double) i;   // compiler emits cvtsi2sd
```

### Why Integers and Floats Need a Special Instruction

You can't just move the bits — the formats are completely different:

```
Integer 42:   00000000 00000000 00000000 00101010  (two's complement)

Double 42.0:  0 10000000100 0101000...000  (sign | exponent | mantissa)
```

The CPU must actually reformat the value, which `cvtsi2sd` does in a single clock cycle.

### Compiler Example

```c
double square(int x) {
    return (double)x * x;
}
```

Compiles to roughly:

```asm
cvtsi2sd  xmm0, edi    ; convert int arg to double
cvtsi2sd  xmm1, edi    ; convert again
mulsd     xmm0, xmm1   ; multiply two doubles
ret
```

### The Full Instruction Family

| Instruction | Converts… | To… |
|-------------|-----------|-----|
| `cvtsi2sd` | signed int | scalar double |
| `cvtsi2ss` | signed int | scalar single (float) |
| `cvtsd2si` | scalar double | signed int |
| `cvtss2si` | scalar single | signed int |
| `cvttsd2si` | scalar double | signed int (truncate) |
| `cvtdq2ps` | packed ints | packed singles |

---

## 6. Why int → double Is Truly "Zero-Cost"

### No Memory Allocation

The conversion happens entirely inside CPU registers — nothing touches RAM:

```
General-Purpose Registers        XMM Registers (SSE)
┌─────────────────────┐          ┌──────────────────────────┐
│  rax  │  42 (int)   │ ──────►  │  xmm0  │  42.0 (double) │
└─────────────────────┘          └──────────────────────────┘
         source                           destination
```

No heap allocation. No stack frame. No `malloc()`. No `new`. Compare to `std::to_string(42)` which does allocate a string on the heap — `cvtsi2sd` allocates nothing because registers are not "memory" in the allocator sense.

### No OS Involvement

A normal allocation path:

```
your code → malloc() → heap allocator → brk()/mmap() → OS kernel
                                                            │
                                                       page table update
                                                       memory mapping
                                                       context switch?
```

`cvtsi2sd` path:

```
your code → cvtsi2sd   ← that's it. never leaves userspace.
```

No syscall. No ring-0 transition. No kernel scheduler involvement.

### No Hidden Work Triggered

| Operation | Looks cheap? | Hidden cost |
|-----------|-------------|-------------|
| `(double)i` → `cvtsi2sd` | ✅ actually cheap | none |
| `(double*)malloc(8)` | ❌ expensive | syscall, lock, bookkeeping |
| Accessing a new memory page | ❌ expensive | page fault, OS handler |
| Virtual function call | ❌ moderate | vtable pointer chase, branch mispredict |
| `std::any` cast | ❌ moderate | `type_info` comparison, possible alloc |

### What "The Instruction Itself" Actually Costs

```
cvtsi2sd xmm0, rax  (Intel Skylake / AMD Zen 3)
├── Latency:    ~4 cycles        (result ready 4 cycles later)
├── Throughput: 1 per cycle      (CPU can start a new one every cycle)
└── Execution units: 1          (uses FP conversion unit)

For context:
├── Integer add (add rax, rbx):  1 cycle latency
├── Memory load (mov rax, [p]):  4 cycles  (L1 cache hit)
└── Division (div):              20–90 cycles latency
```

### Complete List of What Does NOT Happen

```
✗  No heap allocation       (no malloc/free)
✗  No stack allocation      (no push/pop/sub rsp)
✗  No function call         (no call/ret, no ABI overhead)
✗  No system call           (no int 0x80, no syscall instruction)
✗  No cache miss            (registers have no cache)
✗  No branch                (no conditional logic)
✗  No lock / mutex          (registers are per-core, not shared)
✗  No garbage collection    (no GC write barrier)
✗  No exception path        (no try/catch machinery)
✗  No RTTI                  (no type_info lookup)

✓  One instruction.
✓  One execution unit.
✓  ~4 cycles.
✓  Done.
```

---

## 7. The Reverse Direction — double → int with cvttsd2si

Going the other direction uses a different instruction:

```c
double i = 42.0;
int d = (int) i;
```

```asm
cvttsd2si  eax, xmm0    ; Convert with Truncation: Scalar Double → Signed Int
```

Notice the extra `t` — `cvtt` instead of `cvt`. That `t` matters a lot.

### Truncation vs. Rounding

| Instruction | Mode | 3.9 → | -3.9 → |
|-------------|------|--------|---------|
| `cvtsd2si` | Rounded (nearest) | 4 | -4 |
| `cvttsd2si` | Truncated (toward zero) | 3 | -3 |

C's `(int)` cast is defined as truncation, so the compiler always picks `cvttsd2si`.

```c
(int)  3.9  →   3    (drops the .9)
(int) -3.9  →  -3    (drops the .9, moves toward zero)
(int)  3.0  →   3    (exact, no issue)
```

It is not rounding — it always chops toward zero.

### Why This Direction Is Dangerous

`int → double` is always safe: a 32-bit int has at most 31 significant bits, and a double has 52 bits of mantissa — it can represent every int exactly.

`double → int` is the dangerous direction:

```
double has range: ±1.8 × 10³⁰⁸
int    has range: −2,147,483,648  to  2,147,483,647
```

Three bad things can happen:

```c
(int) 42.0         →  42          ✅ fine
(int) 42.9         →  42          ✅ fine (truncation)
(int) 2147483648.0 →  ???         ❌ overflow  (too big for int)
(int) 1e300        →  ???         ❌ overflow  (way too big)
(int) NaN          →  ???         ❌ undefined
(int) Infinity     →  ???         ❌ undefined
```

### The "Integer Indefinite" Sentinel — What the CPU Does on Overflow

The x86 CPU doesn't crash or throw — it returns a special sentinel value:

```
0x80000000             = −2,147,483,648  (32-bit int)
0x8000000000000000     (64-bit long)
```

```c
double huge = 1e300;
int d = (int) huge;
printf("%d\n", d);   // prints -2147483648  ← silent corruption, no crash
```

No exception. No signal. Silently wrong.

---

## 8. Deep Dive: cvttsd2si on 21474836495.2

This walks through every bit of a real overflow case.

### Step 1 — Represent 21474836495 in Binary

```
2^34 = 17,179,869,184
21,474,836,495 − 17,179,869,184 = 4,294,967,311

2^32 = 4,294,967,296
4,294,967,311 − 4,294,967,296 = 15  =  2^3 + 2^2 + 2^1 + 2^0

So: 21,474,836,495 = 2^34 + 2^32 + 2^3 + 2^2 + 2^1 + 2^0
Binary: 1_0100_0000_0000_0000_0000_0000_0000_1111
```

### Step 2 — Normalize to IEEE 754 Scientific Binary

```
1_0100_0000_0000_0000_0000_0000_0000_1111.0010...
= 1.0100000000000000000000000000001111_0010... × 2^34
         ↑
    drop the leading 1 (implicit in IEEE 754)
```

The `.2` fractional part (0.2 in binary = 0.0011 0011 0011... repeating) appends to the mantissa after the integer bits.

```
52-bit mantissa:
0100 0000 0000 0000 0000 0000 0000 1111 0011 0011 0011 0011
|_________________52 bits___________________________________|
```

### Step 3 — Encode the Exponent

```
Actual exponent = 34
IEEE 754 bias   = 1023
Stored exponent = 34 + 1023 = 1057 = 100_0010_0001 in binary
```

### Step 4 — Full IEEE 754 Bit Pattern

```
S  Exponent (11 bits)  Mantissa (52 bits)
──────────────────────────────────────────────────────────────
0  100 0010 0001       0100 0000 0000 0000 0000 0000 0000 1111 0011 0011 0011 0011

Hex: 0x4220_0000_0000_0F33
```

### Step 5 — cvttsd2si Executes Step by Step

```
1. Extract sign bit        →  0  (positive)
2. Decode exponent         →  1057 − 1023 = 34  →  scaled by 2^34
3. Restore implicit 1      →  1.0100000000000000000000000000001111 0011...
4. Shift left by 34        →  move decimal point 34 places right
   integer part: 1_0100_0000_0000_0000_0000_0000_0000_1111 = 21,474,836,495 ✓
5. Truncate fractional bits →  .0011 0011...  DROPPED
6. Check range              →  21,474,836,495 > 2,147,483,647 (INT_MAX) ❌ OVERFLOW
7. Write sentinel value     →  eax = 0x80000000 = −2,147,483,648
```

```
                          ┌──────────────┐
xmm0  [ 21474836495.2 ] ─►│  cvttsd2si   │──►  eax = 0x80000000 = −2,147,483,648
                          └──────────────┘               ≠ 21,474,836,495
                                                         SILENT CORRUPTION ⚠️
```

Note: the `.2` fractional part is irrelevant here — the integer part alone already overflows. The CPU detects this during range check and returns the Integer Indefinite sentinel with no exception, no signal, no warning.

### Fix: Use long Instead

```c
long d = (long) i;   // compiler emits:  cvttsd2si rax, xmm0  (64-bit)
```

```
64-bit int range: −9,223,372,036,854,775,808  to  9,223,372,036,854,775,807
21,474,836,495 fits comfortably ✓
rax = 21,474,836,495   ← correct, truncated (drops the .2)
```

---

## 9. Safe Conversion in Practice

If you care about correctness, you need to guard it yourself — the CPU will not protect you:

```c
// C — manual range check
if (i >= INT_MIN && i <= INT_MAX && !isnan(i)) {
    d = (int) i;
}

// C++ — safer cast (throws on failure)
d = boost::numeric_cast<int>(i);  // throws bad_numeric_cast if out of range
```# Type Conversion in C++ & x86 — Complete Guide

---

## 1. What Is static_cast?

`static_cast` is a compile-time type conversion operator in C++. It is not a system call — it never touches the OS kernel. It is purely a compiler directive that tells the compiler how to reinterpret or convert a value from one type to another.

```cpp
int x = 42;
double d = static_cast<double>(x);  // No syscall. Pure compiler work.
```

---

## 2. The Four Kinds of static_cast

### 2a. Numeric Conversions

```cpp
int i = 100;
double d = static_cast<double>(i);
```

The compiler emits a CPU instruction like `cvtsi2sd` (on x86) to convert an integer register to a floating-point register. No memory allocation, no OS involvement, zero runtime overhead beyond the instruction itself.

### 2b. Pointer Upcasting / Downcasting (Inheritance)

```cpp
class Base { virtual void foo() {} };
class Derived : public Base { void bar() {} };

Derived* d = new Derived();
Base* b = static_cast<Base*>(d);        // upcast — always safe
Derived* d2 = static_cast<Derived*>(b); // downcast — you assert safety
```

For upcasting: the compiler may adjust the pointer value by an offset (in multiple inheritance). For downcasting: no runtime check — you are telling the compiler "trust me, this is a Derived*". Wrong assumption = undefined behavior. Still no syscall — the compiler just adds/subtracts a compile-time constant from the pointer.

**Memory Layout (multiple inheritance example):**

```
┌──────────────────────────┐
│  Base subobject (offset 0)│  ← Base* points here
├──────────────────────────┤
│  Derived members         │  ← Derived* points to start
└──────────────────────────┘

static_cast<Base*>(derived_ptr) = derived_ptr + sizeof(Base_offset)
```

### 2c. void* Conversions

```cpp
void* raw = malloc(100);
int* ip = static_cast<int*>(raw);  // tell compiler: treat this memory as int*
```

The pointer value is unchanged — it's the same address. The compiler just changes how it emits access instructions (byte width, alignment assumptions).

### 2d. enum ↔ Integer

```cpp
enum Color { Red = 0, Green = 1, Blue = 2 };
int n = static_cast<int>(Color::Green);  // → 1
Color c = static_cast<Color>(2);         // → Blue
```

Purely compile-time integer aliasing. Zero overhead.

---

## 3. static_cast vs. Other Casts

| Cast | When | Compile-time? | Runtime check? | Safe? |
|------|------|---------------|----------------|-------|
| `static_cast` | Related types, numeric, pointer hierarchy | ✅ Yes | ❌ No | ⚠️ Mostly |
| `dynamic_cast` | Polymorphic downcasting | Partial | ✅ Yes (RTTI) | ✅ Yes |
| `reinterpret_cast` | Raw bit reinterpretation | ✅ Yes | ❌ No | ❌ Dangerous |
| `const_cast` | Add/remove const | ✅ Yes | ❌ No | ⚠️ Careful |
| C-style `(T)x` | Any of the above | Varies | Varies | ❌ Unpredictable |

### static_cast vs dynamic_cast — The Key Difference

```cpp
Base* b = new Base();  // NOT a Derived!

// static_cast — blind trust, no check:
Derived* d1 = static_cast<Derived*>(b);  // compiles fine → UB at runtime!

// dynamic_cast — uses RTTI, checks vtable at runtime:
Derived* d2 = dynamic_cast<Derived*>(b); // returns nullptr safely
```

`dynamic_cast` does have runtime overhead — it walks the RTTI (Run-Time Type Information) tables the compiler embeds in your binary. Still no syscall, but it's not free.

---

## 4. What Happens at the OS/Kernel Level?

Absolutely nothing. Here's the full execution chain:

```
static_cast<T>(x)
       │
       ▼
  [Compiler validates legality at compile time]
       │
       ▼
  [Emits CPU instruction: convert/adjust/reinterpret]
       │
       ▼
  [CPU executes in a single clock cycle or less]
       │
       ▼
  [OS/Kernel: never woken up, never consulted]
```

| Layer | What Happens |
|-------|-------------|
| C++ Source | You write `static_cast<T>(x)` |
| Compiler (Clang/GCC) | Validates the cast at compile time, emits appropriate machine instructions |
| CPU | Executes a register conversion instruction (e.g. `cvtsi2sd`, `movsx`, `lea`) |
| OS/Kernel | 💤 Not involved at all |

The OS only gets involved when you do things like `malloc` (→ `brk`/`mmap` syscall), file I/O, threading, etc. A type cast is pure ALU/register work.

---

## 5. The cvtsi2sd Instruction — Integer → Double

`cvtsi2sd` is the SSE2 instruction the compiler emits for `int → double` conversion.

### Anatomy of the Name

| Part | Meaning |
|------|---------|
| `cvt` | Convert |
| `si` | Signed Integer |
| `2` | To |
| `sd` | Scalar Double-precision |

### What It Does

```asm
cvtsi2sd xmm0, rax
```

- Reads a signed integer from a general-purpose register (e.g. `rax`, `eax`)
- Converts it to a 64-bit double
- Writes the result into the lower 64 bits of an XMM register (e.g. `xmm0`)

In C terms:

```c
int i = 42;
double d = (double) i;   // compiler emits cvtsi2sd
```

### Why Integers and Floats Need a Special Instruction

You can't just move the bits — the formats are completely different:

```
Integer 42:   00000000 00000000 00000000 00101010  (two's complement)

Double 42.0:  0 10000000100 0101000...000  (sign | exponent | mantissa)
```

The CPU must actually reformat the value, which `cvtsi2sd` does in a single clock cycle.

### Compiler Example

```c
double square(int x) {
    return (double)x * x;
}
```

Compiles to roughly:

```asm
cvtsi2sd  xmm0, edi    ; convert int arg to double
cvtsi2sd  xmm1, edi    ; convert again
mulsd     xmm0, xmm1   ; multiply two doubles
ret
```

### The Full Instruction Family

| Instruction | Converts… | To… |
|-------------|-----------|-----|
| `cvtsi2sd` | signed int | scalar double |
| `cvtsi2ss` | signed int | scalar single (float) |
| `cvtsd2si` | scalar double | signed int |
| `cvtss2si` | scalar single | signed int |
| `cvttsd2si` | scalar double | signed int (truncate) |
| `cvtdq2ps` | packed ints | packed singles |

---

## 6. Why int → double Is Truly "Zero-Cost"

### No Memory Allocation

The conversion happens entirely inside CPU registers — nothing touches RAM:

```
General-Purpose Registers        XMM Registers (SSE)
┌─────────────────────┐          ┌──────────────────────────┐
│  rax  │  42 (int)   │ ──────►  │  xmm0  │  42.0 (double) │
└─────────────────────┘          └──────────────────────────┘
         source                           destination
```

No heap allocation. No stack frame. No `malloc()`. No `new`. Compare to `std::to_string(42)` which does allocate a string on the heap — `cvtsi2sd` allocates nothing because registers are not "memory" in the allocator sense.

### No OS Involvement

A normal allocation path:

```
your code → malloc() → heap allocator → brk()/mmap() → OS kernel
                                                            │
                                                       page table update
                                                       memory mapping
                                                       context switch?
```

`cvtsi2sd` path:

```
your code → cvtsi2sd   ← that's it. never leaves userspace.
```

No syscall. No ring-0 transition. No kernel scheduler involvement.

### No Hidden Work Triggered

| Operation | Looks cheap? | Hidden cost |
|-----------|-------------|-------------|
| `(double)i` → `cvtsi2sd` | ✅ actually cheap | none |
| `(double*)malloc(8)` | ❌ expensive | syscall, lock, bookkeeping |
| Accessing a new memory page | ❌ expensive | page fault, OS handler |
| Virtual function call | ❌ moderate | vtable pointer chase, branch mispredict |
| `std::any` cast | ❌ moderate | `type_info` comparison, possible alloc |

### What "The Instruction Itself" Actually Costs

```
cvtsi2sd xmm0, rax  (Intel Skylake / AMD Zen 3)
├── Latency:    ~4 cycles        (result ready 4 cycles later)
├── Throughput: 1 per cycle      (CPU can start a new one every cycle)
└── Execution units: 1          (uses FP conversion unit)

For context:
├── Integer add (add rax, rbx):  1 cycle latency
├── Memory load (mov rax, [p]):  4 cycles  (L1 cache hit)
└── Division (div):              20–90 cycles latency
```

### Complete List of What Does NOT Happen

```
✗  No heap allocation       (no malloc/free)
✗  No stack allocation      (no push/pop/sub rsp)
✗  No function call         (no call/ret, no ABI overhead)
✗  No system call           (no int 0x80, no syscall instruction)
✗  No cache miss            (registers have no cache)
✗  No branch                (no conditional logic)
✗  No lock / mutex          (registers are per-core, not shared)
✗  No garbage collection    (no GC write barrier)
✗  No exception path        (no try/catch machinery)
✗  No RTTI                  (no type_info lookup)

✓  One instruction.
✓  One execution unit.
✓  ~4 cycles.
✓  Done.
```

---

## 7. The Reverse Direction — double → int with cvttsd2si

Going the other direction uses a different instruction:

```c
double i = 42.0;
int d = (int) i;
```

```asm
cvttsd2si  eax, xmm0    ; Convert with Truncation: Scalar Double → Signed Int
```

Notice the extra `t` — `cvtt` instead of `cvt`. That `t` matters a lot.

### Truncation vs. Rounding

| Instruction | Mode | 3.9 → | -3.9 → |
|-------------|------|--------|---------|
| `cvtsd2si` | Rounded (nearest) | 4 | -4 |
| `cvttsd2si` | Truncated (toward zero) | 3 | -3 |

C's `(int)` cast is defined as truncation, so the compiler always picks `cvttsd2si`.

```c
(int)  3.9  →   3    (drops the .9)
(int) -3.9  →  -3    (drops the .9, moves toward zero)
(int)  3.0  →   3    (exact, no issue)
```

It is not rounding — it always chops toward zero.

### Why This Direction Is Dangerous

`int → double` is always safe: a 32-bit int has at most 31 significant bits, and a double has 52 bits of mantissa — it can represent every int exactly.

`double → int` is the dangerous direction:

```
double has range: ±1.8 × 10³⁰⁸
int    has range: −2,147,483,648  to  2,147,483,647
```

Three bad things can happen:

```c
(int) 42.0         →  42          ✅ fine
(int) 42.9         →  42          ✅ fine (truncation)
(int) 2147483648.0 →  ???         ❌ overflow  (too big for int)
(int) 1e300        →  ???         ❌ overflow  (way too big)
(int) NaN          →  ???         ❌ undefined
(int) Infinity     →  ???         ❌ undefined
```

### The "Integer Indefinite" Sentinel — What the CPU Does on Overflow

The x86 CPU doesn't crash or throw — it returns a special sentinel value:

```
0x80000000             = −2,147,483,648  (32-bit int)
0x8000000000000000     (64-bit long)
```

```c
double huge = 1e300;
int d = (int) huge;
printf("%d\n", d);   // prints -2147483648  ← silent corruption, no crash
```

No exception. No signal. Silently wrong.

---

## 8. Deep Dive: cvttsd2si on 21474836495.2

This walks through every bit of a real overflow case.

### Step 1 — Represent 21474836495 in Binary

```
2^34 = 17,179,869,184
21,474,836,495 − 17,179,869,184 = 4,294,967,311

2^32 = 4,294,967,296
4,294,967,311 − 4,294,967,296 = 15  =  2^3 + 2^2 + 2^1 + 2^0

So: 21,474,836,495 = 2^34 + 2^32 + 2^3 + 2^2 + 2^1 + 2^0
Binary: 1_0100_0000_0000_0000_0000_0000_0000_1111
```

### Step 2 — Normalize to IEEE 754 Scientific Binary

```
1_0100_0000_0000_0000_0000_0000_0000_1111.0010...
= 1.0100000000000000000000000000001111_0010... × 2^34
         ↑
    drop the leading 1 (implicit in IEEE 754)
```

The `.2` fractional part (0.2 in binary = 0.0011 0011 0011... repeating) appends to the mantissa after the integer bits.

```
52-bit mantissa:
0100 0000 0000 0000 0000 0000 0000 1111 0011 0011 0011 0011
|_________________52 bits___________________________________|
```

### Step 3 — Encode the Exponent

```
Actual exponent = 34
IEEE 754 bias   = 1023
Stored exponent = 34 + 1023 = 1057 = 100_0010_0001 in binary
```

### Step 4 — Full IEEE 754 Bit Pattern

```
S  Exponent (11 bits)  Mantissa (52 bits)
──────────────────────────────────────────────────────────────
0  100 0010 0001       0100 0000 0000 0000 0000 0000 0000 1111 0011 0011 0011 0011

Hex: 0x4220_0000_0000_0F33
```

### Step 5 — cvttsd2si Executes Step by Step

```
1. Extract sign bit        →  0  (positive)
2. Decode exponent         →  1057 − 1023 = 34  →  scaled by 2^34
3. Restore implicit 1      →  1.0100000000000000000000000000001111 0011...
4. Shift left by 34        →  move decimal point 34 places right
   integer part: 1_0100_0000_0000_0000_0000_0000_0000_1111 = 21,474,836,495 ✓
5. Truncate fractional bits →  .0011 0011...  DROPPED
6. Check range              →  21,474,836,495 > 2,147,483,647 (INT_MAX) ❌ OVERFLOW
7. Write sentinel value     →  eax = 0x80000000 = −2,147,483,648
```

```
                          ┌──────────────┐
xmm0  [ 21474836495.2 ] ─►│  cvttsd2si   │──►  eax = 0x80000000 = −2,147,483,648
                          └──────────────┘               ≠ 21,474,836,495
                                                         SILENT CORRUPTION ⚠️
```

Note: the `.2` fractional part is irrelevant here — the integer part alone already overflows. The CPU detects this during range check and returns the Integer Indefinite sentinel with no exception, no signal, no warning.

### Fix: Use long Instead

```c
long d = (long) i;   // compiler emits:  cvttsd2si rax, xmm0  (64-bit)
```

```
64-bit int range: −9,223,372,036,854,775,808  to  9,223,372,036,854,775,807
21,474,836,495 fits comfortably ✓
rax = 21,474,836,495   ← correct, truncated (drops the .2)
```

---

## 9. Safe Conversion in Practice

If you care about correctness, you need to guard it yourself — the CPU will not protect you:

```c
// C — manual range check
if (i >= INT_MIN && i <= INT_MAX && !isnan(i)) {
    d = (int) i;# Type Conversion in C++ & x86 — Complete Guide

---

## 1. What Is static_cast?

`static_cast` is a compile-time type conversion operator in C++. It is not a system call — it never touches the OS kernel. It is purely a compiler directive that tells the compiler how to reinterpret or convert a value from one type to another.

```cpp
int x = 42;
double d = static_cast<double>(x);  // No syscall. Pure compiler work.
```

---

## 2. The Four Kinds of static_cast

### 2a. Numeric Conversions

```cpp
int i = 100;
double d = static_cast<double>(i);
```

The compiler emits a CPU instruction like `cvtsi2sd` (on x86) to convert an integer register to a floating-point register. No memory allocation, no OS involvement, zero runtime overhead beyond the instruction itself.

### 2b. Pointer Upcasting / Downcasting (Inheritance)

```cpp
class Base { virtual void foo() {} };
class Derived : public Base { void bar() {} };

Derived* d = new Derived();
Base* b = static_cast<Base*>(d);        // upcast — always safe
Derived* d2 = static_cast<Derived*>(b); // downcast — you assert safety
```

For upcasting: the compiler may adjust the pointer value by an offset (in multiple inheritance). For downcasting: no runtime check — you are telling the compiler "trust me, this is a Derived*". Wrong assumption = undefined behavior. Still no syscall — the compiler just adds/subtracts a compile-time constant from the pointer.

**Memory Layout (multiple inheritance example):**

```
┌──────────────────────────┐
│  Base subobject (offset 0)│  ← Base* points here
├──────────────────────────┤
│  Derived members         │  ← Derived* points to start
└──────────────────────────┘

static_cast<Base*>(derived_ptr) = derived_ptr + sizeof(Base_offset)
```

### 2c. void* Conversions

```cpp
void* raw = malloc(100);
int* ip = static_cast<int*>(raw);  // tell compiler: treat this memory as int*
```

The pointer value is unchanged — it's the same address. The compiler just changes how it emits access instructions (byte width, alignment assumptions).

### 2d. enum ↔ Integer

```cpp
enum Color { Red = 0, Green = 1, Blue = 2 };
int n = static_cast<int>(Color::Green);  // → 1
Color c = static_cast<Color>(2);         // → Blue
```

Purely compile-time integer aliasing. Zero overhead.

---

## 3. static_cast vs. Other Casts

| Cast | When | Compile-time? | Runtime check? | Safe? |
|------|------|---------------|----------------|-------|
| `static_cast` | Related types, numeric, pointer hierarchy | ✅ Yes | ❌ No | ⚠️ Mostly |
| `dynamic_cast` | Polymorphic downcasting | Partial | ✅ Yes (RTTI) | ✅ Yes |
| `reinterpret_cast` | Raw bit reinterpretation | ✅ Yes | ❌ No | ❌ Dangerous |
| `const_cast` | Add/remove const | ✅ Yes | ❌ No | ⚠️ Careful |
| C-style `(T)x` | Any of the above | Varies | Varies | ❌ Unpredictable |

### static_cast vs dynamic_cast — The Key Difference

```cpp
Base* b = new Base();  // NOT a Derived!

// static_cast — blind trust, no check:
Derived* d1 = static_cast<Derived*>(b);  // compiles fine → UB at runtime!

// dynamic_cast — uses RTTI, checks vtable at runtime:
Derived* d2 = dynamic_cast<Derived*>(b); // returns nullptr safely
```

`dynamic_cast` does have runtime overhead — it walks the RTTI (Run-Time Type Information) tables the compiler embeds in your binary. Still no syscall, but it's not free.

---

## 4. What Happens at the OS/Kernel Level?

Absolutely nothing. Here's the full execution chain:

```
static_cast<T>(x)
       │
       ▼
  [Compiler validates legality at compile time]
       │
       ▼
  [Emits CPU instruction: convert/adjust/reinterpret]
       │
       ▼
  [CPU executes in a single clock cycle or less]
       │
       ▼
  [OS/Kernel: never woken up, never consulted]
```

| Layer | What Happens |
|-------|-------------|
| C++ Source | You write `static_cast<T>(x)` |
| Compiler (Clang/GCC) | Validates the cast at compile time, emits appropriate machine instructions |
| CPU | Executes a register conversion instruction (e.g. `cvtsi2sd`, `movsx`, `lea`) |
| OS/Kernel | 💤 Not involved at all |

The OS only gets involved when you do things like `malloc` (→ `brk`/`mmap` syscall), file I/O, threading, etc. A type cast is pure ALU/register work.

---

## 5. The cvtsi2sd Instruction — Integer → Double

`cvtsi2sd` is the SSE2 instruction the compiler emits for `int → double` conversion.

### Anatomy of the Name

| Part | Meaning |
|------|---------|
| `cvt` | Convert |
| `si` | Signed Integer |
| `2` | To |
| `sd` | Scalar Double-precision |

### What It Does

```asm
cvtsi2sd xmm0, rax
```

- Reads a signed integer from a general-purpose register (e.g. `rax`, `eax`)
- Converts it to a 64-bit double
- Writes the result into the lower 64 bits of an XMM register (e.g. `xmm0`)

In C terms:

```c
int i = 42;
double d = (double) i;   // compiler emits cvtsi2sd
```

### Why Integers and Floats Need a Special Instruction

You can't just move the bits — the formats are completely different:

```
Integer 42:   00000000 00000000 00000000 00101010  (two's complement)

Double 42.0:  0 10000000100 0101000...000  (sign | exponent | mantissa)
```

The CPU must actually reformat the value, which `cvtsi2sd` does in a single clock cycle.

### Compiler Example

```c
double square(int x) {
    return (double)x * x;
}
```

Compiles to roughly:

```asm
cvtsi2sd  xmm0, edi    ; convert int arg to double
cvtsi2sd  xmm1, edi    ; convert again
mulsd     xmm0, xmm1   ; multiply two doubles
ret
```

### The Full Instruction Family

| Instruction | Converts… | To… |
|-------------|-----------|-----|
| `cvtsi2sd` | signed int | scalar double |
| `cvtsi2ss` | signed int | scalar single (float) |
| `cvtsd2si` | scalar double | signed int |
| `cvtss2si` | scalar single | signed int |
| `cvttsd2si` | scalar double | signed int (truncate) |
| `cvtdq2ps` | packed ints | packed singles |

---

## 6. Why int → double Is Truly "Zero-Cost"

### No Memory Allocation

The conversion happens entirely inside CPU registers — nothing touches RAM:

```
General-Purpose Registers        XMM Registers (SSE)
┌─────────────────────┐          ┌──────────────────────────┐
│  rax  │  42 (int)   │ ──────►  │  xmm0  │  42.0 (double) │
└─────────────────────┘          └──────────────────────────┘
         source                           destination
```

No heap allocation. No stack frame. No `malloc()`. No `new`. Compare to `std::to_string(42)` which does allocate a string on the heap — `cvtsi2sd` allocates nothing because registers are not "memory" in the allocator sense.

### No OS Involvement

A normal allocation path:

```
your code → malloc() → heap allocator → brk()/mmap() → OS kernel
                                                            │
                                                       page table update
                                                       memory mapping
                                                       context switch?
```

`cvtsi2sd` path:

```
your code → cvtsi2sd   ← that's it. never leaves userspace.
```

No syscall. No ring-0 transition. No kernel scheduler involvement.

### No Hidden Work Triggered

| Operation | Looks cheap? | Hidden cost |
|-----------|-------------|-------------|
| `(double)i` → `cvtsi2sd` | ✅ actually cheap | none |
| `(double*)malloc(8)` | ❌ expensive | syscall, lock, bookkeeping |
| Accessing a new memory page | ❌ expensive | page fault, OS handler |
| Virtual function call | ❌ moderate | vtable pointer chase, branch mispredict |
| `std::any` cast | ❌ moderate | `type_info` comparison, possible alloc |

### What "The Instruction Itself" Actually Costs

```
cvtsi2sd xmm0, rax  (Intel Skylake / AMD Zen 3)
├── Latency:    ~4 cycles        (result ready 4 cycles later)
├── Throughput: 1 per cycle      (CPU can start a new one every cycle)
└── Execution units: 1          (uses FP conversion unit)

For context:
├── Integer add (add rax, rbx):  1 cycle latency
├── Memory load (mov rax, [p]):  4 cycles  (L1 cache hit)
└── Division (div):              20–90 cycles latency
```

### Complete List of What Does NOT Happen

```
✗  No heap allocation       (no malloc/free)
✗  No stack allocation      (no push/pop/sub rsp)
✗  No function call         (no call/ret, no ABI overhead)
✗  No system call           (no int 0x80, no syscall instruction)
✗  No cache miss            (registers have no cache)
✗  No branch                (no conditional logic)
✗  No lock / mutex          (registers are per-core, not shared)
✗  No garbage collection    (no GC write barrier)
✗  No exception path        (no try/catch machinery)
✗  No RTTI                  (no type_info lookup)

✓  One instruction.
✓  One execution unit.
✓  ~4 cycles.
✓  Done.
```

---

## 7. The Reverse Direction — double → int with cvttsd2si

Going the other direction uses a different instruction:

```c
double i = 42.0;
int d = (int) i;
```

```asm
cvttsd2si  eax, xmm0    ; Convert with Truncation: Scalar Double → Signed Int
```

Notice the extra `t` — `cvtt` instead of `cvt`. That `t` matters a lot.

### Truncation vs. Rounding

| Instruction | Mode | 3.9 → | -3.9 → |
|-------------|------|--------|---------|
| `cvtsd2si` | Rounded (nearest) | 4 | -4 |
| `cvttsd2si` | Truncated (toward zero) | 3 | -3 |

C's `(int)` cast is defined as truncation, so the compiler always picks `cvttsd2si`.

```c
(int)  3.9  →   3    (drops the .9)
(int) -3.9  →  -3    (drops the .9, moves toward zero)
(int)  3.0  →   3    (exact, no issue)
```

It is not rounding — it always chops toward zero.

### Why This Direction Is Dangerous

`int → double` is always safe: a 32-bit int has at most 31 significant bits, and a double has 52 bits of mantissa — it can represent every int exactly.

`double → int` is the dangerous direction:

```
double has range: ±1.8 × 10³⁰⁸
int    has range: −2,147,483,648  to  2,147,483,647
```

Three bad things can happen:

```c
(int) 42.0         →  42          ✅ fine
(int) 42.9         →  42          ✅ fine (truncation)
(int) 2147483648.0 →  ???         ❌ overflow  (too big for int)
(int) 1e300        →  ???         ❌ overflow  (way too big)
(int) NaN          →  ???         ❌ undefined
(int) Infinity     →  ???         ❌ undefined
```

### The "Integer Indefinite" Sentinel — What the CPU Does on Overflow

The x86 CPU doesn't crash or throw — it returns a special sentinel value:

```
0x80000000             = −2,147,483,648  (32-bit int)
0x8000000000000000     (64-bit long)
```

```c
double huge = 1e300;
int d = (int) huge;
printf("%d\n", d);   // prints -2147483648  ← silent corruption, no crash
```

No exception. No signal. Silently wrong.

---

## 8. Deep Dive: cvttsd2si on 21474836495.2

This walks through every bit of a real overflow case.

### Step 1 — Represent 21474836495 in Binary

```
2^34 = 17,179,869,184
21,474,836,495 − 17,179,869,184 = 4,294,967,311

2^32 = 4,294,967,296
4,294,967,311 − 4,294,967,296 = 15  =  2^3 + 2^2 + 2^1 + 2^0

So: 21,474,836,495 = 2^34 + 2^32 + 2^3 + 2^2 + 2^1 + 2^0
Binary: 1_0100_0000_0000_0000_0000_0000_0000_1111
```

### Step 2 — Normalize to IEEE 754 Scientific Binary

```
1_0100_0000_0000_0000_0000_0000_0000_1111.0010...
= 1.0100000000000000000000000000001111_0010... × 2^34
         ↑
    drop the leading 1 (implicit in IEEE 754)
```

The `.2` fractional part (0.2 in binary = 0.0011 0011 0011... repeating) appends to the mantissa after the integer bits.

```
52-bit mantissa:
0100 0000 0000 0000 0000 0000 0000 1111 0011 0011 0011 0011
|_________________52 bits___________________________________|
```

### Step 3 — Encode the Exponent

```
Actual exponent = 34
IEEE 754 bias   = 1023
Stored exponent = 34 + 1023 = 1057 = 100_0010_0001 in binary
```

### Step 4 — Full IEEE 754 Bit Pattern

```
S  Exponent (11 bits)  Mantissa (52 bits)
──────────────────────────────────────────────────────────────
0  100 0010 0001       0100 0000 0000 0000 0000 0000 0000 1111 0011 0011 0011 0011

Hex: 0x4220_0000_0000_0F33
```

### Step 5 — cvttsd2si Executes Step by Step

```
1. Extract sign bit        →  0  (positive)
2. Decode exponent         →  1057 − 1023 = 34  →  scaled by 2^34
3. Restore implicit 1      →  1.0100000000000000000000000000001111 0011...
4. Shift left by 34        →  move decimal point 34 places right
   integer part: 1_0100_0000_0000_0000_0000_0000_0000_1111 = 21,474,836,495 ✓
5. Truncate fractional bits →  .0011 0011...  DROPPED
6. Check range              →  21,474,836,495 > 2,147,483,647 (INT_MAX) ❌ OVERFLOW
7. Write sentinel value     →  eax = 0x80000000 = −2,147,483,648
```

```
                          ┌──────────────┐
xmm0  [ 21474836495.2 ] ─►│  cvttsd2si   │──►  eax = 0x80000000 = −2,147,483,648
                          └──────────────┘               ≠ 21,474,836,495
                                                         SILENT CORRUPTION ⚠️
```

Note: the `.2` fractional part is irrelevant here — the integer part alone already overflows. The CPU detects this during range check and returns the Integer Indefinite sentinel with no exception, no signal, no warning.

### Fix: Use long Instead

```c
long d = (long) i;   // compiler emits:  cvttsd2si rax, xmm0  (64-bit)
```

```
64-bit int range: −9,223,372,036,854,775,808  to  9,223,372,036,854,775,807
21,474,836,495 fits comfortably ✓
rax = 21,474,836,495   ← correct, truncated (drops the .2)
```

---

## 9. Safe Conversion in Practice

If you care about correctness, you need to guard it yourself — the CPU will not protect you:

```c
// C — manual range check
if (i >= INT_MIN && i <= INT_MAX && !isnan(i)) {
    d = (int) i;
}

// C++ — safer cast (throws on failure)
d = boost::numeric_cast<int>(i);  // throws bad_numeric_cast if out of range
```
}

// C++ — safer cast (throws on failure)
d = boost::numeric_cast<int>(i);  // throws bad_numeric_cast if out of range
```