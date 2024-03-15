def bytewise_or(s1, s2):
   
    bytes1 = s1.split('-')
    bytes2 = s2.split('-')

    # for each couple of byte compute the logical OR and format in hex 
    result_bytes = [format(int(byte1, 16) | int(byte2, 16), '02x') for byte1, byte2 in zip(bytes1, bytes2)]
    result = '-'.join(result_bytes)

    return result


def bytewise_and(s1, s2):
    
    bytes1 = s1.split('-')
    bytes2 = s2.split('-')

    result_bytes = [format(int(byte1, 16) & int(byte2, 16), '02x') for byte1, byte2 in zip(bytes1, bytes2)]
    result = '-'.join(result_bytes)

    return result


def main():

    rand1 = "63-3b-6d-07-65-1a-09-31-7a-4f-b4-aa-ef-3f-7a-55-d0-33-93-52-1e-81-fb-63-11-26-ed-9e-8e-a7-10-f6-63-9d-eb-92-90-eb-76-0b-90-5a-eb-b4-75-d3-a1-cf-d2-91-39-c1-89-32-84-22-12-4e-77-57-4d-25-85-98"
    rand2 = "92-05-d8-b5-fa-85-97-b6-22-f4-bd-26-11-cf-79-8c-db-4a-28-27-bb-d3-31-56-74-16-df-cb-f5-61-a7-9d-18-c2-63-92-f1-cb-c3-6d-2b-77-19-aa-21-07-8e-fe-8b-1a-4f-7d-70-6e-a4-7b-c8-68-30-43-12-50-30-1e"

    k1 = bytewise_or(rand1, rand2)
    k2 = bytewise_and(rand1, rand2)

    int_k1 = int(k1.replace('-', ''), 16)
    int_k2 = int(k2.replace('-', ''), 16)

    # Perform bitwise XOR operation and format in hex
    k3 = int_k1 ^ int_k2
    hex_k3 = format(k3, 'x')

    # Insert hyphens every two characters

    res = '-'.join(hex_k3[i:i+2] for i in range(0, len(hex_k3), 2))

    print(res)


main()