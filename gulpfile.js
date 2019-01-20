const gulp = require('gulp');
const gulpLoadPlugins = require('gulp-load-plugins');

const $ = gulpLoadPlugins();

// Publishes the site to GitHub Pages
gulp.task('publish', () => {
  console.log('Publishing to GH Pages');
  return gulp.src('./.vuepress/dist/**/*')
    .pipe($.ghPages({
      remoteUrl: `https://matusnovak:${process.env.GITHUB_TOKEN}@github.com/matusnovak/finegraphics.git`,
      origin: 'origin',
      branch: 'gh-pages'
    }));
});
