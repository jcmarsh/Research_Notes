
fn main() {
    println!("Hello, world!");

    let test_message = [0x48,0x65,0x6C,0x6C,0x6F,0x20,0x54,0x68,0x65,0x72,0x65,0x21];
    let test_same = [0x48,0x65,0x6C,0x6C,0x6F,0x20,0x54,0x68,0x65,0x72,0x65,0x21];
    let test_different = [0x48,0x65,0x6C,0x6F,0x6C,0x20,0x54,0x68,0x65,0x72,0x65,0x21];

    if test_message == test_same {
        println!("Oh, neat.");
    }

    if test_message == test_different {
        println!("Oh, well, hmmm.");
    }

    // Test a slice...?
    if test_message[0..3] == test_different[0..3] {
        println!("Well then, that was easy.");
    }
}