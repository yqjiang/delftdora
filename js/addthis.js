/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


(function($) {
    Drupal.behaviors.delftdoraSolr = {
        attach: function(context, settings) {
            $(document).ready(function() {
                $('.shareoptions').find('.share').find('a').addClass("addthis_button");
                $('.shareoptions').find('.share').find('a').attr("href", "http://www.addthis.com/bookmark.php?v=300&amp;pubid=ra-5281f8c33c6dd728");
                $('body').append('<script type="text/javascript">var addthis_config = {"data_track_addressbar":true};</script>');
                $('body').append('<script type="text/javascript" src="//s7.addthis.com/js/300/addthis_widget.js#pubid=ra-5281f8c33c6dd728"></script>');
            });

        }
    };
})(jQuery);
