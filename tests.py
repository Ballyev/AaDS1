import subprocess
import base64
import unittest

class TestASCII85Decoder(unittest.TestCase):
    def test_basic_decoding(self):
        test_data = b"Hello"
        encoded = base64.a85encode(test_data).decode('ascii')

        result = subprocess.run(
            ["./build/ascii85_decoder"],
            input=encoded,
            capture_output=True,
            text=True
        )

        self.assertEqual(result.returncode, 0)
        self.assertEqual(result.stdout.encode('latin1'), test_data)

if __name__ == 'main':
    unittest.main()