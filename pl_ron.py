##### Question 4 #####

mat1 = [[0,1], [2,3]]
mat2 = [[6],[7]]


def colcat(mat1, mat2):
    if len(mat1) == 0:
        return []
    if len(mat1) == 1:
        return [mat1[0] + mat2[0]]
    return [mat1[0] + mat2[0]] + colcat(mat1[1:], mat2[1:])

# print(colcat(mat2, mat1))


def vertical_split(input_mat):
    a = len(input_mat[0])//2 # number of columns
    # print(a)
    return vertical_split_helper(input_mat, a)

def vertical_split_helper(input_mat, a):
    if(len(input_mat) == 0):
        return []
    if(len(input_mat) == 1):
        return ([input_mat[0][:a]], [input_mat[0][a:]])
    return ([input_mat[0][:a]] + vertical_split_helper(input_mat[1:], a)[0], 
            [input_mat[0][a:]] + vertical_split_helper(input_mat[1:], a)[1])

in_mat =[[1,2,3,4],[5,6,7,8],[9,10,11,12],[13,14,15,16]]
# print(vertical_split(in_mat))

def rotate_mat_rec(input_mat):
    if len(input_mat) == 0:
        return []
    if len(input_mat) == 1:
        return [input_mat[0]]
    split = vertical_split(input_mat)
    # print(split[0][0])
    # print(split[0][1])


    # mat1 = [split[0][1]] + [split[1][1]]
    # mat2 = [split[0][0]] + [split[1][0]]
    len_split = len(split[0])//2
    mat1 = rotate_mat_rec(split[0][:len_split])
    mat2 = rotate_mat_rec(split[0][len_split:])
    mat3 = rotate_mat_rec(split[1][:len_split])
    mat4 = rotate_mat_rec(split[1][len_split:])

    # print(mat1)
    # print(mat2)
    # print(mat3)
    # print(mat4)
    return colcat(mat2+mat4, mat1+mat3)
    # return colcat(mat1, mat2)



in_mat_rot = [[1,2],[3,4]]
in_mat_rot2 =[[1,2,3,4],[5,6,7,8],[9,10,11,12],[13,14,15,16]]
# print(rotate_mat_rec(in_mat_rot2))



def merge_zigzag(list1,list2):
    if(len(list1) == 0 and len(list2) == 0):
        return []
    if(len(list1) == 0):
        zigzag1 = [list2[0]] + merge_zigzag_helper(list1, list2[1:], list2[0], True)
        zigzag2 = [list2[0]] + merge_zigzag_helper(list1, list2[1:], list2[0], False)
        if(len(zigzag1) > len(zigzag2)):
            return zigzag1
        else:
            return zigzag2
        
    if(len(list2) == 0):
        zigzag1 = [list1[0]] + merge_zigzag_helper(list1[1:], list2, list1[0], True)
        zigzag2 = [list1[0]] + merge_zigzag_helper(list1[1:], list2, list1[0], False)
        if(len(zigzag1) > len(zigzag2)):
            return zigzag1
        else:
            return zigzag2
    
    zigzag2 = [list2[0]] + merge_zigzag_helper(list1, list2[1:], list2[0], True)
    zigzag1 = [list1[0]] + merge_zigzag_helper(list1[1:], list2, list1[0], True)
    zigzag3 = [list1[0]] + merge_zigzag_helper(list1[1:], list2, list1[0], False)
    zigzag4 = [list2[0]] + merge_zigzag_helper(list1, list2[1:], list2[0], False)
    # zigzag5 = merge_zigzag(list1[1:],list2[1:])
    # zigzag6 = merge_zigzag(list1,list2[1:])
    # zigzag7 = merge_zigzag(list1[1:],list2)
    zigzag5 = merge_zigzag(list1[1:],list2[1:])
    zigzag6 = merge_zigzag(list1,list2[1:])
    zigzag7 = merge_zigzag(list1[1:],list2)
    
    

    length_zizag = [(zigzag1, len(zigzag1)), (zigzag2, len(zigzag2)), (zigzag3, len(zigzag3)), (zigzag4, len(zigzag4)), (zigzag5, len(zigzag5)), (zigzag6, len(zigzag6)), (zigzag7, len(zigzag7))]
    length_zizag.sort(key=lambda x: x[1], reverse=True)
    return length_zizag[0][0]
    

    
list1 = [15,10,5,3]
list2 = [2,22,13,14]
# list1 = [1,3,5,23,11]
# list2 = [2,0,6,17,18]



def merge_zigzag_helper(list1, list2, last_value, is_bigger):
    zigzag3 = []
    zigzag2 = []
    zigzag1 = []

    if(len(list1) == 0 and len(list2) == 0):
        return []
    if(len(list1) == 0):
        if(is_bigger):      
            

            if(list2[0] < 0.5 * last_value):
                zigzag2 = [list2[0]] + merge_zigzag_helper(list1, list2[1:], list2[0], False)
            else:                
                zigzag3 = merge_zigzag_helper(list1, list2[1:], last_value, True)   
                return zigzag3
            zigzag3 = merge_zigzag_helper(list1, list2[1:], last_value, True)   
            if(len(zigzag2) > len(zigzag3)):                
                    return zigzag2
            else:                
                    return zigzag3  
        else:
            
            if(list2[0] >2 * last_value):
                zigzag2 = [list2[0]] + merge_zigzag_helper(list1, list2[1:], list2[0], True)
            else:       
                zigzag3 = merge_zigzag_helper(list1, list2[1:], last_value, False)         
                return zigzag3

            zigzag3 = merge_zigzag_helper(list1, list2[1:], last_value, False)
            if(len(zigzag2) > len(zigzag3)):                
                    return zigzag2
            else:                
                    return zigzag3  
            
    if(len(list2) == 0):
        if(is_bigger):      
            
            if(list1[0] < 0.5 * last_value):
                zigzag2 = [list1[0]] + merge_zigzag_helper(list1[1:], list2, list1[0], False)
            else:       
                zigzag3 = merge_zigzag_helper(list1[1:], list2, last_value, True)              
                return zigzag3
            zigzag3 = merge_zigzag_helper(list1[1:], list2, last_value, True)     
            if(len(zigzag2) > len(zigzag3)):                
                    return zigzag2
            else:                
                    return zigzag3  
        else:
            
            if(list1[0] >2 * last_value):
                zigzag2 = [list1[0]] + merge_zigzag_helper(list1[1:], list2, list1[0], True)
            else:
                zigzag3 = merge_zigzag_helper(list1[1:], list2, last_value, False)
                return zigzag3
            zigzag3 = merge_zigzag_helper(list1[1:], list2, last_value, False)
            if(len(zigzag2) > len(zigzag3)):                
                    return zigzag2
            else:                
                    return zigzag3  




    if(is_bigger):
        if(list2[0] < 0.5 * last_value):
            zigzag2 = [list2[0]] + merge_zigzag_helper(list1, list2[1:], list2[0], False)
        if(list1[0] < 0.5 * last_value):
            zigzag1 = [list1[0]] + merge_zigzag_helper(list1[1:], list2, list1[0], False)
        
        zigzag3 = merge_zigzag_helper(list1[1:], list2[1:], last_value, True)

        if(len(zigzag1) > len(zigzag2)):
            if(len(zigzag1) > len(zigzag3)):
                return zigzag1
            else:
                 return zigzag3
        else:
            if(len(zigzag3) > len(zigzag2)):
                return zigzag3
            else:
                return zigzag2
    else:
        if(list2[0] > 2 * last_value):
            zigzag2 = [list2[0]] + merge_zigzag_helper(list1, list2[1:], list2[0], True)
        if(list1[0] >2 * last_value):
            zigzag1 = [list1[0]] + merge_zigzag_helper(list1[1:], list2, list1[0], True)
        
        zigzag3 = merge_zigzag_helper(list1[1:], list2[1:], last_value, False)

        if(len(zigzag1) > len(zigzag2)):
            if(len(zigzag1) > len(zigzag3)):
                return zigzag1
            else:
                 return zigzag3
        else:
            if(len(zigzag3) > len(zigzag2)):
                return zigzag3
            else:
                return zigzag2

print(merge_zigzag(list1, list2))