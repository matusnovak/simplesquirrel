const ghpages = require("gh-pages")

console.log("Publishing to GH Pages")
ghpages.publish('.vuepress/dist', {
  repo: 'https://' + process.env.GITHUB_TOKEN + '@github.com/matusnovak/simplesquirrel.git'
}, function (err) {
  if (err) {
    console.log(err)
    process.exit(1)
  }
})
