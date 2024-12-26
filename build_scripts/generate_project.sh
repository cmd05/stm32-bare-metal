# refer page 46 um1718-stm32cubemx-for-stm32-configuration-and-initialization-c-code-generation-stmicroelectronics.pdf for script commands

set -e

if [ -z "$1" ]
then
    echo "Specify STM32CubeMX project name"
    exit 1
fi

# create project
mkdir $1
cp default_gen.ioc ./$1/
echo "$(cat ./cubemx_gen_script.mxgen)" | sed "s/__PROJECT_NAME__/$1/g" > ./$1/temp.mxgen

cd $1
powershell.exe -Command java -jar $(where stm32cubemx) -q ./temp.mxgen # bash: stm32cubemx -q ./temp.mxgen > gen_mx.log

# initialize vscode workspace tasks
cp -rf "/C/Users/aquila/Documents/Repositories/Embedded_Dev/config-vscode-stm/.vscode" . && \
    echo "[SCRIPT_LOG]: Created .vscode workspace for $1"

# cleanup tasks
rm ./temp.mxgen
mv default_gen.ioc $1.ioc
cd ..