#!/bin/bash
reserveds=(
# コメントアウトしているものはうまく生成出来ないので手動
    ':'
    '/'
#    '?'
    '#'
    '['
    ']'
    '@'
    '!'
    '$'
    '&'
    "'"
    '('
    ')'
#    '*'
    '+'
    ','
    ';'
    '='
    '%'
    ' '
)
IFS=$'\n'
for i in ${reserveds[*]};
do

echo "'$i.html' generating..."

cat << EOF > $i.html
<!DOCTYPE html>
    <head>
        <meta charset="utf-8">
    </head>
    <body>
        <h1>'$i.html'</h1>
        <a href="./$i.html#$i" title="permalink">$i</a>
    </body>
</html>
EOF
done
