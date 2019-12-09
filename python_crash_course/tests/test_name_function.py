import unittest
from name_function import get_formatted_name

class NamesTestCase(unittest.TestCase):
    """Tests for 'name_function.py'."""

    def test_first_last_name(self):
        """Test names like 'Janis Joplin'"""
        formatted_name = get_formatted_name('janis', 'joplin')
        self.assertEqual(formatted_name, 'Janis Joplin')

    def test_three_names(self):
        """Test names like 'Lee Harvey Oswald'"""
        formatted_name = get_formatted_name('lee', 'harvey', 'oswald')
        self.assertEqual(formatted_name, 'Lee Harvey Oswald')

if __name__ == '__main__':
    unittest.main()
