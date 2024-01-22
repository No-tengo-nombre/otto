from _otto import lib as otto


def main():
    a = otto.otto_buffer_new(5, None) # This fails but I only care that it works
    print(a)
