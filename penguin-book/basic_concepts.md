# Fundamentals

## Vaiables
Variables are one the fundamentals in a programming language . Variables are defined as a containter that store values. 
the way one should create a variables in penguin , given below

`name = "penguin"`

Penguin is a dynamically typed language , that means there is no need to mention the type while creating  . Types get assigneed automatically after assigning .

`name = "penguin"` this is a line which stores a `string` `"penguin"` in a variable called `name`

## Data Types

Even though we don't write type while creating or assigning values in variables . Each variable do have types . 
The following a\are Primitive Data Types 
- int 
- float
- char
- string
- bool

## Size of Data Types
- int : Integer values are 64-bit/ 8 Bytes by default
- float : Float values are 64-bit/ 8 Bytes by default
- char : Character is 1 byte
- bool : bool is 1 byte

## Time to code
```rust
{
    func main(){
        a = 10;
        b = 10.44;
        c = "Hello Universe !";
        d = 'Z';
        e = false;
        println(type(a));
        println(type(b));
        println(type(c));
        println(type(d));
        println(type(e));
    }
}
```
**Output**
```bash 
int
float
string
char
bool
```
*Note :* 
- `type` is a built-in function to print the type of a variable
- `println` is a built-in function to print with a new- ine .
- `prinbt` is a built-in function to print without a new line.

## Operations 

Penguin supports the common operators used for calculations, comparisons, logic, bitwise work, and updating variables.

### Arithmetic Operators

Arithmetic operators work on integer values.

| Operator | Meaning | Example |
| --- | --- | --- |
| `+` | Addition | `12 + 5` |
| `-` | Subtraction | `12 - 5` |
| `*` | Multiplication | `12 * 5` |
| `/` | Division | `12 / 5` |
| `%` | Remainder | `12 % 5` |

```rust
{
    func main(){
        println("Arithmetic operators");
        println("12 + 5 = {12 + 5}");
        println("12 - 5 = {12 - 5}");
        println("12 * 5 = {12 * 5}");
        println("12 / 5 = {12 / 5}");
        println("12 % 5 = {12 % 5}");

    }
}

```

### String Concatenation

Use `+` to join two strings.

```rust
first = "Hello";
second = " Penguin";
greeting = first + second;
println(greeting);
```

### Comparison Operators

Comparison operators return a boolean value.

| Operator | Meaning |
| --- | --- |
| `>` | Greater than |
| `>=` | Greater than or equal |
| `<` | Less than |
| `<=` | Less than or equal |
| `==` | Equal to |
| `!=` | Not equal to |

### Logical Operators

Logical operators are used with boolean values and conditions.

| Operator | Meaning | Example |
| --- | --- | --- |
| `&&` | Logical AND | `age >= 18 && has_id == true` |
| `||` | Logical OR | `day == 6 || day == 7` |
| `!` | Logical NOT | `!false` |

### Bitwise Operators

Bitwise operators work on integer bits.

| Operator | Meaning | Example |
| --- | --- | --- |
| `&` | Bitwise AND | `6 & 3` |
| <code>|</code> | Bitwise OR | `6 | 3` |
| `^` | Bitwise XOR | `6 ^ 3` |
| `<<` | Left shift | `6 << 1` |
| `>>` | Right shift | `6 >> 1` |

### Compound Assignment Operators

Compound assignment operators update an existing variable.

| Operator | Same as |
| --- | --- |
| `+=` | `x = x + value` |
| `-=` | `x = x - value` |
| `*=` | `x = x * value` |
| `/=` | `x = x / value` |
| `%=` | `x = x % value` |
| `&=` | `x = x & value` |
| <code>|=</code> | `x = x | value` |
| `^=` | `x = x ^ value` |

### Precedence

Operators with higher precedence run first. Parentheses can be used when you want a different order.

```rust
println(2 + 3 * 4);     // 14
println((2 + 3) * 4);   // 20
```

See `examples/operators.pg` for a complete runnable example.
