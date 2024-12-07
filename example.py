import sys

def fibonacci(n):
    """Generate Fibonacci sequence up to n."""
    sequence = []
    a, b = 0, 1
    while a < n:
        sequence.append(a)
        a, b = b, a + b
    return sequence

def main():
    if len(sys.argv) != 2:
        print("Usage: python example_script.py <number>")
        sys.exit(1)

    try:
        number = int(sys.argv[1])
        if number <= 0:
            raise ValueError("Number must be positive.")
    except ValueError as e:
        print(f"Invalid input: {e}")
        sys.exit(1)

    result = fibonacci(number)
    print(f"Fibonacci sequence up to {number}: {result}")

    with open("output.txt", "w") as f:
        f.write("Fibonacci sequence:\n")
        f.write(", ".join(map(str, result)))

if __name__ == "__main__":
    main()