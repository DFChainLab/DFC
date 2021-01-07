The following steps must be taken for the example script to work.

0. Create wallet
0. Create account for dfcio.token
0. Create account for scott
0. Create account for exchange
0. Set token contract on dfcio.token
0. Create DFC token
0. Issue initial tokens to scott

**Note**:
Deleting the `transactions.txt` file will prevent replay from working.


### Create wallet
`cldfc wallet create`

### Create account steps
`cldfc create key`

`cldfc create key`

`cldfc wallet import  --private-key <private key from step 1>`

`cldfc wallet import  --private-key <private key from step 2>`

`cldfc create account dfcio <account_name> <public key from step 1> <public key from step 2>`

### Set contract steps
`cldfc set contract dfcio.token /contracts/dfcio.token -p dfcio.token@active`

### Create DFC token steps
`cldfc push action dfcio.token create '{"issuer": "dfcio.token", "maximum_supply": "100000.0000 DFC", "can_freeze": 1, "can_recall": 1, "can_whitelist": 1}' -p dfcio.token@active`

### Issue token steps
`cldfc push action dfcio.token issue '{"to": "scott", "quantity": "900.0000 DFC", "memo": "testing"}' -p dfcio.token@active`
