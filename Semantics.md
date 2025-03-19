# Abyssian

## Syntax

### Basic Constructions
1. Variables Declaration
```
<variable name> = <expression>

npcHealth = 100
npcName = "Guard"
```

2. Conditions
```
if <condition>
    # statements
elif <condition>
    # statements
else
    # statements
end
```

3. Print to Console
```
print <expression>
print npcHealth
```

4. Input from Console
```
input <variable name>
input playerName
```

5. Functions
```
fun <function name>(<argument1>, <argument2>, ...)
    <body>
    return <expression>
end

fun healNPC(npc, amount)
    npc.health = npc.health + amount
    return npc.health
end
```

6. Loops
```
# For loop
for <variable> in <lower_bound>..<upper_bound>
    # statements
end

# While loop
while <condition>
    # statements
end

# For-each loop
foreach <variable> in <collection>
    # statements
end
```

### Data Types
1. Numeric
```
number1 = 1
number2 = 3.14
number3 = -5
```

2. Text
```
text = "hello world"
```

3. Logical
```
logical1 = true
logical2 = false
```

4. Object
```
object npc
npc.health = 100
npc.name = "Guard"
```

5. Arrays
```
<array> = [<value1>, <value2>, ...]
npcList = [npc1, npc2, npc3]
```

### Specific Constructs for NPC and Event Control
1. Event Listener
```
on <event> do
    # statements
end

on npcDeath do
    print "NPC has died"
end
```

2. NPC Action
```
npc <npc_name> do
    # statements
end

npc Guard do
    npc.health = 0
    print npc.name + " has " + npc.health + " health"
end
```

### Operators
| Operator       | Symbol | Precedence | Example         |
| -------------- | ------ | ---------- | --------------- |
| Assignment     | `=`    | 1          | `a = 5`         |
| Logical OR     | `or`   | 2          | `true or false` |
| Logical AND    | `and`  | 3          | `true and true` |
| Equals         | `==`   | 4          | `a == 5`        |
| Not Equals     | `!=`   | 4          | `a != 5`        |
| Greater Than   | `>`    | 5          | `a > 5`         |
| Less Than      | `<`    | 5          | `a < 5`         |
| Addition       | `+`    | 6          | `a + 5`         |
| Subtraction    | `-`    | 6          | `a - 5`         |
| Multiplication | `*`    | 7          | `a * 5`         |
| Division       | `/`    | 7          | `a / 5`         |
| NOT            | `not`  | 8          | `not false`     |