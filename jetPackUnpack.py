  
def extractKBits(num,k,p): 
  
  # convert number into binary first 
  binary = bin(num) 
  
  # remove first two characters 
  binary = binary[2:] 
  
  end = len(binary) - p 
  start = end - k + 1
  start = 0 if start < 0 else start
  
  # extract k  bit sub-string 
  kBitSubStr = binary[start : end+1] 
  
  # convert extracted sub-string into decimal again 
  return int(kBitSubStr,2)
  
def unpackJet(hexJetValue):
  jetValue = int(hexJetValue, 16)
  pt = extractKBits(jetValue, 16, 1) # 16 bits from the first pos
  phi = extractKBits(jetValue, 8, 17) # 10 bits from the 11th pos
  eta = extractKBits(jetValue, 8, 25) # 10 bits from the 21th pos
  print("packed => pt, eta, phi:", hexJetValue, "=>", pt, eta, phi)

def packJet(pt, eta, phi):
  jetValue = pt + (phi << 16) + (eta << 24)
  hexJetValue = hex(jetValue)
  packedWord = "1v0000000000000000"
  # remove 0x from jet, count how many character, remove that number of characters from the tail of the base word and replace them with the jet
  packedWord = packedWord[:-len(hexJetValue[2:])] + hexJetValue[2:]
  return packedWord

def packPfInput(pt, eta, phi):
  jetValue = int(pt/0.25) + (int(phi/0.0043633231) << 16) + (int(eta/0.0043633231) << 26)
  hexJetValue = hex(jetValue)
  packedWord = "1v0000000000000000"
  # remove 0x from jet, count how many character, remove that number of characters from the tail of the base word and replace them with the jet
  packedWord = packedWord[:-len(hexJetValue[2:])] + hexJetValue[2:]
  return packedWord

def unpackPfInput(hexJetValue):
  jetValue = int(hexJetValue, 16)
  pt = extractKBits(jetValue, 16, 1) # 16 bits from the first pos
  phi = extractKBits(jetValue, 10, 17) # 10 bits from the 11th pos
  eta = extractKBits(jetValue, 10, 27) # 10 bits from the 21th pos
  print("packed => pt, eta, phi:", hexJetValue, "=>", pt, eta, phi)
