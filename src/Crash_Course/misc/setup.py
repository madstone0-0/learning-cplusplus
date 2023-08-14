from distutils.core import setup, Extension

spam = Extension("spam", sources=["spammodule.cpp"])
setup(name="spam", version="1.0", description="SPAM", ext_modules=[spam])
