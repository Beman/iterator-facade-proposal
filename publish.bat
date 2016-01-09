@echo off
copy /y styles.css gh-pages
copy /y iterator-facade-proposal.html gh-pages
cd gh-pages
git commit -a --message="%date%"
git push
cd ..
 