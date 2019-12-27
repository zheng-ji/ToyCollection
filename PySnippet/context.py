class OpenContext(object):
    def __init__(self, filename, mode):
        self.fp = open(filename, mode)
    
    def __enter__(self):
        return self.fp
    def __exit__(self, exc_type, exc_val, exc_tb):
        self.fp.close()

with OpenContext("/tmp/a", 'a') as f:
    f.write("helloworld")

