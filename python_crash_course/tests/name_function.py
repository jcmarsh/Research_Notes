def get_formatted_name(*names):
    """Generate a neatly formatted full name."""
    full_name = ""
    for name in names:
        full_name = full_name + name + " "

    return full_name.rstrip().title()
