def wrap_string(string_val):
    """utility function to strip unprintable characters
    from a string before printing.
    """
    chars = [ ord(c) for c in string_val ]
    chars = [ chr(c) if c < 128 else "?" for c in chars ]
    return ''.join(chars)

