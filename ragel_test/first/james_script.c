
#line 1 "james_script.rl"
#include 
#include 


#line 11 "james_script.rl"



#line 12 "james_script.c"
static const char _james_script_key_offsets[] = {
	0, 0, 2, 7
};

static const char _james_script_trans_keys[] = {
	48, 57, 32, 9, 13, 48, 57, 32, 
	9, 13, 48, 57, 0
};

static const char _james_script_single_lengths[] = {
	0, 0, 1, 1
};

static const char _james_script_range_lengths[] = {
	0, 1, 2, 2
};

static const char _james_script_index_offsets[] = {
	0, 0, 2, 6
};

static const char _james_script_trans_targs[] = {
	3, 0, 2, 2, 3, 0, 2, 2, 
	3, 0, 0
};

static const int james_script_start = 1;
static const int james_script_first_final = 3;
static const int james_script_error = 0;

static const int james_script_en_main = 1;


#line 14 "james_script.rl"

static uint8_t cs; /* The current parser state */

void init_james_script( void ) {
  
#line 52 "james_script.c"
	{
	cs = james_script_start;
	}

#line 19 "james_script.rl"
}

void parse_james_script(const char* p, uint16_t len, uint8_t is_eof) {
  const char* pe = p + len; /* pe points to 1 byte beyond the end of this block of data */
  char* eof = is_eof ? pe : ((char*) 0); /* Indicates the end of all data, 0 if not in this block */
  
  
#line 65 "james_script.c"
	{
	int _klen;
	unsigned int _trans;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _james_script_trans_keys + _james_script_key_offsets[cs];
	_trans = _james_script_index_offsets[cs];

	_klen = _james_script_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _james_script_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	cs = _james_script_trans_targs[_trans];

	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 26 "james_script.rl"
}
