/**
 * @file
 * Javascript file for islandora solr search facets
 */

(function ($) {
  "use strict";
  // Adds facet toggle functionality
  Drupal.behaviors.delftdoraSolr = {
    attach: function (context, settings) {
      var limit = settings.delftdoraSolr.displayLimit;
      if (!$(".show-more").hasClass('processed')) {
        $(".show-more").click(function (e) {
          var items = $(this).parent().find('li:gt(' + limit + ')');
          var self = $(this),
            timer = 400,
            label = self.find('b');
          if (items.is(':hidden')) {
            items.show(timer);
            label.text(label.attr('data-hide-text'));
          } else {
            items.hide(timer);
            label.text(label.attr('data-show-text'));
          }
        });
        $(".show-more").addClass('processed');
      }
    }
  };
})(jQuery);
