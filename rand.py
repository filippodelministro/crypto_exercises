
def or_fun(string1, string2):
    s1 = string1.split('-')
    s2 = string2.split('-')

    result = []

    for i, j in zip(s1, s2):
        a = int(i, 16)
        b = int(j, 16)
        result.append(a|b)

    return result

def and_fun(string1, string2):
    s1 = string1.split('-')
    s2 = string2.split('-')

    result = []

    for i, j in zip(s1, s2):
        a = int(i, 16)
        b = int(j, 16)
        result.append(a&b)

    return result

def xor_fun(k1, k2):
    result = []

    for i, j in zip(k1, k2):
        #d = hex(i^j)
        h = format(i^j, 'x')
        result.append(h)

    result_string = "-".join(result)
    return result_string


def main():

    #rand1 = "63-3b"
    #rand2 = "92-05"
    rand1 = "63-3b-6d-07-65-1a-09-31-7a-4f-b4-aa-ef-3f-7a-55-d0-33-93-52-1e-81-fb-63-11-26-ed-9e-8e-a7-10-f6-63-9d-eb-92-90-eb-76-0b-90-5a-eb-b4-75-d3-a1-cf-d2-91-39-c1-89-32-84-22-12-4e-77-57-4d-25-85-98"
    rand2 = "92-05-d8-b5-fa-85-97-b6-22-f4-bd-26-11-cf-79-8c-db-4a-28-27-bb-d3-31-56-74-16-df-cb-f5-61-a7-9d-18-c2-63-92-f1-cb-c3-6d-2b-77-19-aa-21-07-8e-fe-8b-1a-4f-7d-70-6e-a4-7b-c8-68-30-43-12-50-30-1e"

    k1 = or_fun(rand1, rand2)
    k2 = and_fun(rand1, rand2)
    k3 = xor_fun(k1, k2)

    #print("k1: ", k1, "\n\nk2", k2)
    print("CRYPTO_23{"+k3+"}")

main()