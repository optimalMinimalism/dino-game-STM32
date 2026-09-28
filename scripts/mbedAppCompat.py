# File: scripts/mbed_python_compat.py
import sys
import collections

# retro-compatibility script
# Patch per collezioni Python 3.10+ usate dal vecchio build system Mbed in PlatformIO
if not hasattr(collections, 'Mapping'):
    import collections.abc
    collections.Mapping = collections.abc.Mapping
if not hasattr(collections, 'MutableMapping'):
    import collections.abc
    collections.MutableMapping = collections.abc.MutableMapping
if not hasattr(collections, 'Sequence'):
    import collections.abc
    collections.Sequence = collections.abc.Sequence