@echo off
html_include_files iterator-facade-proposal.md iterator-facade-proposal-0.md
pandoc --css=styles.css --toc --toc-depth=5 -f markdown_github+pipe_tables -t html -o iterator-facade-proposal.temp iterator-facade-proposal-0.md
sed "s:%%!{:<em>:g;s:}!%%:</em>:g" <iterator-facade-proposal.temp >iterator-facade-proposal-2.temp
html_section_numbers --initial=24.8.8 <iterator-facade-proposal-2.temp >iterator-facade-proposal.html
iterator-facade-proposal.html
