# Abyssian

Abyssian is a programming language designed to facilitate the creation and management of Non-Player Characters (NPCs) and their interactions within a game environment. It offers a straightforward syntax and specialized constructs tailored for game development, enabling developers to script complex behaviors and dialogues with ease.

## Features

- **Intuitive Syntax:** Abyssian's syntax is designed for clarity and simplicity, making it accessible for both novice and experienced developers.
- **Game-Centric Constructs:** The language includes specialized constructs for defining NPCs, dialogues, and interactions, streamlining the game development process.
- **Flexible Data Types:** Supports various data types, including numeric, text, logical, objects, and arrays, allowing for versatile programming paradigms.
- **Structured Flow Control:** Includes conditions, loops, and function support for robust scripting capabilities.
- **Object-Oriented Design:** Allows for object definition and manipulation, enhancing NPC behavior complexity.

## Syntax Overview

### Variable Declaration

```abyssian
<variable_name> = <expression>

npcHealth = 100
npcName = "Guard"
```

### Conditions

```abyssian
if <condition>
    # statements
elif <condition>
    # statements
else
    # statements
end
```

### Console Output

```abyssian
print <expression>
print npcHealth
```

### Console Input

```abyssian
input <variable_name>
input playerName
```

### Functions

```abyssian
fun <function_name>(<argument1>, <argument2>, ...)
    <body>
    return <expression>
end

fun healNPC(npc, amount)
    npc.health = npc.health + amount
    return npc.health
end
```

### Loops

#### For Loop

```abyssian
for <variable> in <lower_bound>..<upper_bound>
    # statements
end
```

#### While Loop

```abyssian
while <condition>
    # statements
end
```

#### For-Each Loop

```abyssian
foreach <variable> in <collection>
    # statements
end
```

### Data Types

- **Numeric:** Represents numbers, both integers and floating-point.

  ```abyssian
  number1 = 1
  number2 = 3.14
  number3 = -5
  ```

- **Text:** Represents sequences of characters.

  ```abyssian
  text = "hello world"
  ```

- **Logical:** Represents boolean values.

  ```abyssian
  logical1 = true
  logical2 = false
  ```

- **Object:** Represents entities with attributes.

  ```abyssian
  object npc
  npc.health = 100
  npc.name = "Guard"
  ```

- **Arrays:** Represents ordered collections of values.

  ```abyssian
  <array> = [<value1>, <value2>, ...]
  npcList = [npc1, npc2, npc3]
  ```

## Operators

Abyssian supports a variety of operators:

### Arithmetic Operators

```abyssian
+  # Addition
-  # Subtraction
*  # Multiplication
/  # Division
%  # Modulo
```

### Comparison Operators

```abyssian
==  # Equal to
!=  # Not equal to
>   # Greater than
<   # Less than
>=  # Greater than or equal to
<=  # Less than or equal to
```

### Logical Operators

```abyssian
and  # Logical AND
or   # Logical OR
not  # Logical NOT
```

### Assignment Operators

```abyssian
=   # Assign value
+=  # Add and assign
-=  # Subtract and assign
*=  # Multiply and assign
/=  # Divide and assign
```

## Comments

Abyssian supports both single-line and multi-line comments:

```abyssian
# This is a single-line comment

/*
This is a multi-line comment
spanning multiple lines.
*/
```

## Getting Started

To begin using Abyssian in your game development projects:

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/MelodicAlbuild/Abyssian.git
   ```

2. **Build the Project:**

   Navigate to the project directory and use the provided build scripts or instructions to compile the source code.

3. **Explore Examples:**

   Review the examples and documentation provided in the repository to understand how to implement various features and constructs in Abyssian.

## Contributing

Contributions to Abyssian are welcome. If you have suggestions, bug reports, or improvements, please submit an issue or a pull request on the [GitHub repository](https://github.com/MelodicAlbuild/Abyssian).

## License

This project is licensed under the [MIT License](LICENSE), allowing for permissive use, distribution, and modification.

---

*Note: Abyssian is currently in active development. Features and syntax are subject to change. Please refer to the latest documentation and updates in the repository for the most accurate information.*
