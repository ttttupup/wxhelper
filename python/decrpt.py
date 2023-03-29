import ctypes
import hashlib
import hmac

# pip install pycryptodome
from Crypto.Cipher import AES


def decrypt(password, input_file, out_file):
    password = bytes.fromhex(password.replace(' ', ''))
    with open(input_file, 'rb') as (f):
        blist = f.read()
    print(len(blist))
    salt = blist[:16]
    key = hashlib.pbkdf2_hmac('sha1', password, salt, DEFAULT_ITER, KEY_SIZE)
    first = blist[16:DEFAULT_PAGESIZE]
    mac_salt = bytes([x ^ 58 for x in salt])
    mac_key = hashlib.pbkdf2_hmac('sha1', key, mac_salt, 2, KEY_SIZE)
    hash_mac = hmac.new(mac_key, digestmod='sha1')
    hash_mac.update(first[:-32])
    hash_mac.update(bytes(ctypes.c_int(1)))
    if hash_mac.digest() == first[-32:-12]:
        print('decrypt success')
    else:
        print('password error')
        return
    blist = [blist[i:i + DEFAULT_PAGESIZE] for i in range(DEFAULT_PAGESIZE, len(blist), DEFAULT_PAGESIZE)]
    with open(out_file, 'wb') as (f):
        f.write(SQLITE_FILE_HEADER)
        t = AES.new(key, AES.MODE_CBC, first[-48:-32])
        f.write(t.decrypt(first[:-48]))
        f.write(first[-48:])
        for i in blist:
            t = AES.new(key, AES.MODE_CBC, i[-48:-32])
            f.write(t.decrypt(i[:-48]))
            f.write(i[-48:])


def main():
    password = '565735E30E474DA09250CB5AA047E3940FFA1C6F767C4263B13ABB512933DA49'
    input_file = 'C:/var/Applet.db'
    out_file = 'c:/var/out/Applet.db'
    decrypt(password, input_file, out_file)


if __name__ == '__main__':
    SQLITE_FILE_HEADER = bytes('SQLite format 3', encoding='ASCII') + bytes(1)
    KEY_SIZE = 32
    DEFAULT_PAGESIZE = 4096
    DEFAULT_ITER = 64000
    main()
