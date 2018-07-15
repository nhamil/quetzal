function Test() 
{
    var text = "Hello, World!" 
    print text 
    
    if text == "Hello, World!" 
    {
        print "equal" 
    }
    else 
    {
        print "not equal" 
    }
}

Test() 

=========================

names: 
    0 Test 
    1 text

consts: 
    0 "Hello, World!" 
    1 "equal" 
    2 "not equal" 

code: 
    make_function 0 
        load_const 0 
        store_local 1 // text
        load_name 1 
        print 
        load_name 1 
        load_const 0 
        equal 
        jump_false 4
            load_const 1 
            print 
            jump 3 
            
            load_const 2 
            print 
        return_none 
    end_function 
    store_name 0 // Test 
    load_name 0 
    call_function
    pop_value
    