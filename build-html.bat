@echo off
pandoc --css=styles.css --toc --toc-depth=4 -f markdown_github -t html -o iterator-facade-proposal.temp iterator-facade-proposal.md
sed "s:%%!{:<em>:g;s:}!%%:</em>:g" <iterator-facade-proposal.temp >iterator-facade-proposal-2.temp
html_section_numbers --initial=24.8.8 <iterator-facade-proposal-2.temp >iterator-facade-proposal.html
iterator-facade-proposal.html
